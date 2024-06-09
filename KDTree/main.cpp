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

// Define Macros 
#define DATA_FILE "./dataset/data.json"

std::vector<std::string> parseArtists(const std::string& artistsString) {
    std::vector<std::string> artists;
    std::string artist;
    bool inQuotes = false;
    
    for (char c : artistsString) {
        if (c == '\'' && (artist.empty() || artist.back() != '\\')) {
            inQuotes = !inQuotes;
            if (!inQuotes && !artist.empty()) {
                artists.push_back(artist);
                artist.clear();
            }
        } else if (inQuotes) {
            artist += c;
        }
    }

    return artists;
}

std::vector<Song> readJSON(const std::string& filename) {
    const std::vector<std::string> keys = Song().get_keys();

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
        std::vector<std::string> artists = parseArtists(item["artists"]);
        std::string release_date = item["release_date"];

        songs.push_back(Song(numeric_data, id, name, artists, release_date));
    }
    std::cout << "JSON file read successfully" << std::endl;
    return songs;
}

Song get_song(std::vector<Song> songs, std::string name) {
    for (Song song : songs) {
        if (song.get_name() == name) {
            return song;
        }
    }
    throw std::invalid_argument("Song not found");
}

int main() {
    std::vector<Song> songs = readJSON(DATA_FILE);
    sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        return a.get_name() < b.get_name();
    });
    std::cout << "Number of songs: " << songs.size() << std::endl;

    std::vector<Point> points;
    for (Song song : songs) {
        points.push_back(Point(song));
    }

    // Create a KDTree with the number of dimensions
    KDTree tree(points[0].coords.size(), points);

    // Find the nearest neighbor to a given song
    Song target = get_song(songs, "Clancy Lowered the Boom");
    std::cout << target.get_coordinates().size() << std::endl;
    Point nearest = tree.findNearestNeighbor(Point(target));

    std::cout << "Nearest neighbor to " << target.get_name() << " is " << nearest.song.get_name() << std::endl;
    // Id of the nearest neighbor
    std::cout << "Id: " << nearest.song.get_id() << std::endl;
    return 0;
} 