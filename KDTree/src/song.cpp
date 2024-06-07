#include "song.h"

#include <stdexcept>

Song::Song(std::map<std::string, double> numeric_data,
    std::string id,
    std::string name,
    std::string artists,
    std::string release_date){

        // Load the string data into the object
        this->id = id;
        this->name = name;
        this->artists = artists;
        this->release_date = release_date;

        // Load the numeric data into the object
        for (auto key : keys){
            if (numeric_data.find(key) == numeric_data.end()){
                throw std::invalid_argument("Missing key: " + key);
            }

            // TODO: Convert the numeric data into double if necessary

            this->numeric_data[key] = numeric_data[key];
        }

}

std::vector<double> Song::getCoordinates() const
{
    // Implement a vector with the order defined in the keys vector
    std::vector<double> coords;
    for (auto key : keys){
        coords.push_back(numeric_data.at(key));
    }
    
    return coords;
}
