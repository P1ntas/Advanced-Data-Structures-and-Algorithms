#include "song.h"

Song::Song()
{
    this->numeric_data = {};
    // add 0 as default value for all numeric data keys
    for (std::string key : keys)
    {
        this->numeric_data[key] = 0;
    }

    this->id = "default ID";
    this->name = "default name";
    this->artists = {};
    this->release_date = "default release date";
}
Song::Song(std::map<std::string, double> numeric_data,
           std::string id,
           std::string name,
           std::vector<std::string> artists,
           std::string release_date)
{
    this->numeric_data = numeric_data;
    this->id = id;
    this->name = name;
    this->artists = artists;
    this->release_date = release_date;
}

bool Song::operator<(const Song &song) const
{
    return this->name < song.name;
}
bool Song::operator==(const Song &song) const
{
    return this->name == song.name;
}
bool Song::operator!=(const Song &song) const
{
    return this->name != song.name;
}
bool Song::operator>(const Song &song) const
{
    return this->name > song.name;
}
Song &Song::operator=(const Song &song)
{
    this->numeric_data = song.numeric_data;
    this->id = song.id;
    this->name = song.name;
    this->artists = song.artists;
    this->release_date = song.release_date;
    return *this;
}

std::vector<double> Song::get_coordinates() const
{
    std::vector<double> coordinates;
    for (std::string key : keys)
    {
        coordinates.push_back(this->numeric_data.at(key));
    }
    return coordinates;
}
const std::vector<std::string> Song::get_keys() const
{
    return keys;
}

std::string Song::get_name() const
{
    return name;
}

std::map<std::string, double> Song::get_numeric_data() const
{
    return numeric_data;
}
