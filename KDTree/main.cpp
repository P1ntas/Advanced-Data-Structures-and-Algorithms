#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
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

    std::vector<double> target_numeric = {0.195, 2020, 0.00998, 0.671, 337147, 0.623, 1, 0.00000755, 2, 0.643, -7.161, 1, 70, 0.308, 75.055};
    std::vector<std::string> target_strings = {"Eminem", "5SiZJoLXp3WOl3J4C8IK0d", "Darkness", "2020-01-17"};
    Song target_song(target_numeric, target_strings);
    Point target(target_song);

    Point nearest = tree.findNearestNeighbor(target);
    std::cout << "Nearest neighbor to song '" << target.song.name << "' is '" << nearest.song.name << "'\n";

    int k = 3;
    std::vector<Point> nearestNeighbors = tree.findKNearestNeighbors(target, k);
    std::cout << k << " Nearest neighbors to song '" << target.song.name << "':\n";
    for (const auto& neighbor : nearestNeighbors) {
        std::cout << "'" << neighbor.song.name << "' by " << neighbor.song.artists << "\n";
    }

    return 0;
}
