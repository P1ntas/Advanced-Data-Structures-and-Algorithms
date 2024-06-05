#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "src/song.h"
#include "src/kdtree.h"
#include "nlohmann/json.hpp"
#include <map>

using json = nlohmann::json;

std::vector<Song> readJSON(const std::string& filename) {
    std::vector<std::string> keys = Song().keys;

    // Read the JSON file
    std::cout << "Reading JSON file: " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::cout << "File opened successfully" << std::endl;

    json jsonData;
    file >> jsonData;
    std::vector<Song> songs;

    for (const auto& item : jsonData) {
        std::map<std::string,double> numeric_data = {};
        numeric_data["valence"] = item["valence"];
        numeric_data["year"] = item["year"];
        numeric_data["acousticness"] = item["acousticness"];
        numeric_data["danceability"] = item["danceability"];
        numeric_data["duration_ms"] = item["duration_ms"];
        numeric_data["energy"] = item["energy"];
        numeric_data["explicit"] = item["explicit"];
        numeric_data["instrumentalness"] = item["instrumentalness"];
        numeric_data["key"] = item["key"];
        numeric_data["liveness"] = item["liveness"];
        numeric_data["loudness"] = item["loudness"];
        numeric_data["mode"] = item["mode"];
        numeric_data["popularity"] = item["popularity"];
        numeric_data["speechiness"] = item["speechiness"];
        numeric_data["tempo"] = item["tempo"];

        // Check if all the keys are present
        for (auto key : keys) {
            if (numeric_data.find(key) == numeric_data.end()) {
                throw std::invalid_argument("Missing key: " + key);
            }
        }

        //TODO: Standardize the data
        std::string id = item["id"];
        std::string name = item["name"];
        std::string artists = item["artists"];
        std::string release_date = item["release_date"];

        songs.push_back(Song(
            numeric_data,
            id,
            name,
            artists,
            release_date
        ));

    }
    std::cout << "JSON file read successfully" << std::endl;
    return songs;
}

Song * getSongByName(std::vector<Song> songs, std::string name) {
    // Binary search
    int left = 0;
    int right = songs.size() - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (songs[middle].name == name) {
            std::cout << "Song found: " << name << std::endl;
            std::cout << "Song: " << songs[middle].name << std::endl;
            std::vector<std::string> keys = Song().keys;

            for (auto key : keys) {
                std::cout << key << ": " << songs[middle].numeric_data[key] << std::endl;
            } 

            return &songs[middle];
        }

        if (songs[middle].name < name) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    std::cout << "Song not found: " << name << std::endl;
    return nullptr;
}

int main() {

    // Read the songs from the JSON file
    std::vector<Song> songs = readJSON("./dataset/data.json");

    // Order the songs by name to use binary search later
    std::sort(songs.begin(), songs.end());

    // Create a KD tree with the same number of dimensions as the songs
    KDTree tree(songs[0].keys.size());

    // Insert the songs into the KD tree
    for (const auto& song : songs) {
        tree.insert(
            Point(
                song.getSong()
            )
        );
    }
    
    /*
    1st: retrieve the name of the song
    2nd: retrieve the song from DB (vector of songs) using the name
    3rd: retrieve the song from KDTree using the song
    */

    // Get the song by name
    std::string song_name = "Goat";
    Song * song = getSongByName(songs, song_name);

    

    return 0;
}
