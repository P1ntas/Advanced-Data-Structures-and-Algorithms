#include "song.h"

Song::Song(std::vector<double> numeric_data, std::vector<std::string> string_data)
    : valence(numeric_data[0]), year(static_cast<int>(numeric_data[1])), acousticness(numeric_data[2]),
      danceability(numeric_data[3]), duration_ms(static_cast<int>(numeric_data[4])), energy(numeric_data[5]),
      explicit_(numeric_data[6] != 0), instrumentalness(numeric_data[7]), key(static_cast<int>(numeric_data[8])),
      liveness(numeric_data[9]), loudness(numeric_data[10]), mode(static_cast<int>(numeric_data[11])),
      popularity(static_cast<int>(numeric_data[12])), speechiness(numeric_data[13]), tempo(numeric_data[14]),
      artists(string_data[0]), id(string_data[1]), name(string_data[2]), release_date(string_data[3]) {}

std::vector<double> Song::getCoordinates() const {
    return {valence, acousticness, danceability, energy, instrumentalness, liveness, loudness, speechiness, tempo};
}
