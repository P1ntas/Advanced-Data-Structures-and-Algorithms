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

    auto overall_start = std::chrono::high_resolution_clock::now(); // Start here performance measurement

    // Test setup
    std::vector<Song> songs = readJSON(DATA_FILE);
    sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        return a.get_name() < b.get_name();
    });
    std::cout << "Number of songs: " << songs.size() << std::endl << std::endl;

    std::vector<Point> points;
    for (Song song : songs) {
        points.push_back(Point(song));
    }

    // Find the nearest neighbor to a given song
    Song target = get_song(songs, "Clancy Lowered the Boom");
    Point targetPoint(target);


    // Find the k nearest neighbors to a given song
    int k = 5; 
    k = k + 1;  // +1 to exclude the target itself

    std::function <double(Point, Point)> distance = [](Point a, Point b) {
        double dist = 0;
        for (int i = 0; i < a.coords.size(); i++) {
            dist += pow(a.coords[i] - b.coords[i], 2);
        }
        return sqrt(dist);
    };

    // First approach: Brute force search for each nearest neighbor
    std::vector<Point> nearestNeighbors;

    auto start_1 = std::chrono::high_resolution_clock::now(); // Start here performance measurement
    for (int i = 0; i < k; i++) {
        double minDist = std::numeric_limits<double>::max();
        Point * nearestNeighbor;
        for (int j = 0; j < points.size(); j++) {
            // If already in the nearest neighbors list, skip
            if (std::find(nearestNeighbors.begin(), nearestNeighbors.end(), points[j]) != nearestNeighbors.end()) {
                continue;
            }

            double dist = distance(points[j], targetPoint);
            if (dist < minDist) {
                minDist = dist;
                nearestNeighbor = &points[j];
            }
        }
        nearestNeighbors.push_back(*nearestNeighbor);
    }
    auto end_1 = std::chrono::high_resolution_clock::now(); // End here performance measurement
    auto elapsed_1 = end_1 - start_1;


    sort(nearestNeighbors.begin(), nearestNeighbors.end(), [targetPoint, distance](const Point& a, const Point& b) {
        return distance(a, targetPoint) < distance(b, targetPoint);
    });
    // Output first approach
    std::cout << "5 Nearest neighbors to " << target.get_name() << " are:" << std::endl;
    for (int i = 1; i < k; i++) {
        std::cout << nearestNeighbors[i].song.get_name() << "with id: " << nearestNeighbors[i].song.get_id() << std::endl;
    }
    std::cout << std::endl;


    // Second approach: Logarithmic search for each nearest neighbor
    
    auto start_2 = std::chrono::high_resolution_clock::now(); // Start here performance measurement
    // Vector sorting is O(nlogn) and the loop is O(k)
    sort(points.begin(), points.end(), [targetPoint, distance](const Point& a, const Point& b) {
        return distance(a, targetPoint) < distance(b, targetPoint);
    });
    auto end_2 = std::chrono::high_resolution_clock::now(); // End here performance measurement
    auto elapsed_2 = end_2 - start_2;


    // Output second approach
    std::cout << "5 Nearest neighbors to " << target.get_name() << " are:" << std::endl;
    for (int i = 1; i < k; i++) {
        std::cout << points[i].song.get_name() << "with id: " << points[i].song.get_id() << std::endl;
    }
    std::cout << std::endl;


    // Third approach: KDTree search for each nearest neighbor
    KDTree tree(points[0].coords.size(), points);
    auto start_3 = std::chrono::high_resolution_clock::now(); // Start here performance measurement
    std::vector<Point> kNearest = tree.findKNearestNeighbors(targetPoint, k);
    auto end_3 = std::chrono::high_resolution_clock::now(); // End here performance measurement
    auto elapsed_3 = end_3 - start_3;
    // Output third approach
    std::cout << "5 Nearest neighbors to " << target.get_name() << " are:" << std::endl;
    for (int i = 1; i < k; i++) {
        std::cout << kNearest[i].song.get_name() << "with id: " << kNearest[i].song.get_id() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Brute force approach time: " << elapsed_1.count() << " seconds" << std::endl;
    std::cout << "Logarithmic approach time: " << elapsed_2.count() << " seconds" << std::endl;
    std::cout << "KDTree approach time: " << elapsed_3.count() << " seconds" << std::endl;

    
    auto overall_end = std::chrono::high_resolution_clock::now(); // End here performance measurement
    auto overall_elapsed = overall_end - overall_start;
    std::cout << "Overall time: " << overall_elapsed.count() << " seconds" << std::endl;

    return 0;
} 