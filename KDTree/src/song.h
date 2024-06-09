#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>
#include <map>

class Song
{
private:
    const std::vector<std::string> keys = {
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
        "tempo"};

    std::map<std::string, double> numeric_data;

    std::string id;
    std::string name;
    std::vector<std::string> artists;
    std::string release_date;

public:
    // Destructor
    ~Song() {}

    // Constructor
    Song();
    Song(std::map<std::string, double> numeric_data,
         std::string id,
         std::string name,
         std::vector<std::string> artists,
         std::string release_date);


    /**
     * @brief Overloaded less than operator for comparing two Song objects.
     * @param song The Song object to compare with.
     * @return True if this Song is less than the given Song, false otherwise.
     */
    bool operator<(const Song &song) const;

    /**
     * @brief Overloaded equality operator for comparing two Song objects.
     * @param song The Song object to compare with.
     * @return True if this Song is equal to the given Song, false otherwise.
     */
    bool operator==(const Song &song) const;

    /**
     * @brief Overloaded inequality operator for comparing two Song objects.
     * @param song The Song object to compare with.
     * @return True if this Song is not equal to the given Song, false otherwise.
     */
    bool operator!=(const Song &song) const;

    /**
     * @brief Overloaded greater than operator for comparing two Song objects.
     * @param song The Song object to compare with.
     * @return True if this Song is greater than the given Song, false otherwise.
     */
    bool operator>(const Song &song) const;

    /**
     * @brief Assignment operator for assigning the values of another Song object to this Song object.
     * @param song The Song object to assign from.
     * @return A reference to this Song object after assignment.
     */
    Song &operator=(const Song &song);

    /**
     * @brief Get the coordinates of the Song.
     * @return A vector of doubles representing the coordinates of the Song.
     */
    std::vector<double> get_coordinates() const;

    /**
     * @brief Get the keys of the Song.
     * @return A constant vector of strings representing the keys of the Song.
     */
    const std::vector<std::string> get_keys() const;

    /**
     * @brief Get the name of the Song.
     * @return A string representing the name of the Song.
     */
    std::string get_name() const;

    /**
     * @brief Get the numeric data of the Song.
     * @return A map of strings to doubles representing the numeric data of the Song.
     */
    std::map<std::string, double> get_numeric_data() const;

    /**
     * @brief Get the ID of the Song.
     * @return A string representing the ID of the Song.
     */
    std::string get_id() const;
};

#endif
