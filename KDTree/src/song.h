#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>
#include <map>

class Song {
public:

    const std::vector<std::string> keys = 
    {
        "year",
        "valence",
        "acousticness",
        "danceability",
        "duration_ms",
        "energy",
        "explicit",
        "instrumentalness",
        "key",
        "liveness",
        "loudness",
        "mode",
        "popularity",
        "speechiness",
        "tempo"
    };

    std::map<std::string, double> numeric_data;
    
    std::string id;
    std::string name;
    std::string artists;
    std::string release_date;

    Song(std::map<std::string, double> numeric_data,
        std::string id, std::string name,
        std::string artists,
        std::string release_date);
    std::vector<double> getCoordinates() const;
};

#endif
