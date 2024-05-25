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

using json = nlohmann::json;

std::vector<Song> readJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    json jsonData;
    file >> jsonData;
    std::vector<Song> songs;

    for (const auto& item : jsonData) {
        std::vector<double> numeric_data = {
            item["valence"],
            item["year"],
            item["acousticness"],
            item["danceability"],
            item["duration_ms"],
            item["energy"],
            item["explicit"],
            item["instrumentalness"],
            item["key"],
            item["liveness"],
            item["loudness"],
            item["mode"],
            item["popularity"],
            item["speechiness"],
            item["tempo"]
        };

        std::vector<std::string> string_data = {
            item["artists"],
            item["id"],
            item["name"],
            item["release_date"]
        };

        songs.push_back(Song(numeric_data, string_data));
    }

    return songs;
}

int main() {
    KDTree tree(15);

    std::vector<Song> songs;
    try {
        songs = readJSON("./dataset/data.json");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error reading JSON file: " << e.what() << std::endl;
        return 1;
    }

    for (const auto& song : songs) {
        tree.insert(Point(song));
    }

    std::string search_name;
    std::string search_artist;
    while (true) {
        std::cout << "Enter the name of the song to search for (or 'exit' to quit): ";
        std::getline(std::cin, search_name);
        if (search_name == "exit") {
            break;
        }

        std::cout << "Enter the artist of the song: ";
        std::getline(std::cin, search_artist);
        if (search_artist == "exit") {
            break;
        }

        auto it = std::find_if(songs.begin(), songs.end(), [&search_name, &search_artist](const Song& song) {
            return song.name == search_name && song.artists == search_artist;
        });

        if (it != songs.end()) {
            Song target_song = *it;
            Point target(target_song);

            Point nearest = tree.findNearestNeighbor(target);
            std::cout << "Nearest neighbor to song '" << target.song.name << "' is '" << nearest.song.name << "'\n";

            int k = 3;
            std::vector<Point> nearestNeighbors = tree.findKNearestNeighbors(target, k);
            std::cout << k << " Nearest neighbors to song '" << target.song.name << "':\n";
            for (const auto& neighbor : nearestNeighbors) {
                std::cout << "'" << neighbor.song.name << "' by " << neighbor.song.artists << "\n";
            }
        } else {
            std::cout << "Song '" << search_name << "' by '" << search_artist << "' not found. Please try again.\n";
        }
    }

    return 0;
}
