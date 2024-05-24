#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>

class Song {
public:
    double valence;
    int year;
    double acousticness;
    std::string artists;
    double danceability;
    int duration_ms;
    double energy;
    bool explicit_;
    std::string id;
    double instrumentalness;
    int key;
    double liveness;
    double loudness;
    int mode;
    std::string name;
    int popularity;
    std::string release_date;
    double speechiness;
    double tempo;

    Song(std::vector<double> numeric_data, std::vector<std::string> string_data);
    std::vector<double> getCoordinates() const;
};

#endif
