#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>
#include <map>

class Song {
public:

    const std::vector<std::string> keys = 
    {
        "valence",
        "year",
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

    const Song* getSong() const {
        return this;
    }

    // Overload the operator to compare two songs
    bool operator==(const Song &song) const {
        return this->id == song.id;
    }

    // Overload the operator to compare two songs
    bool operator!=(const Song &song) const {
        return this->id != song.id;
    }

    // Overload the operator to compare two songs
    bool operator<(const Song &song) const {
        return this->name < song.name;
    }

    // Destructor
    ~Song() {}

    // Copy constructor
    Song(const Song &song) {
        this->numeric_data = song.numeric_data;
        this->id = song.id;
        this->name = song.name;
        this->artists = song.artists;
        this->release_date = song.release_date;
    }

    // Copy assignment operator
    Song& operator=(const Song &song) {
        this->numeric_data = song.numeric_data;
        this->id = song.id;
        this->name = song.name;
        this->artists = song.artists;
        this->release_date = song.release_date;
        return *this;
    }

    // Move constructor
    Song(Song &&song) {
        this->numeric_data = song.numeric_data;
        this->id = song.id;
        this->name = song.name;
        this->artists = song.artists;
        this->release_date = song.release_date;
    }

    // Move assignment operator
    Song& operator=(Song &&song) {
        this->numeric_data = song.numeric_data;
        this->id = song.id;
        this->name = song.name;
        this->artists = song.artists;
        this->release_date = song.release_date;
        return *this;
    }
    
};

#endif
