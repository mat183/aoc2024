#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

struct Antenna {
  char frequency;
  int x;
  int y;
};

int count_unique_locations_for_antinode(
    const std::map<char, std::vector<Antenna>> &antennas, int map_x,
    int map_y) {
  std::set<std::pair<int, int>> unique_locations;

  for (const auto &[frequency, same_freq_antennas] : antennas) {
    if (same_freq_antennas.size() > 1) {
      for (std::size_t i = 0; i < same_freq_antennas.size() - 1; ++i) {
        const auto &antenna_i = same_freq_antennas[i];
        for (std::size_t j = i + 1; j < same_freq_antennas.size(); ++j) {
          const auto &antenna_j = same_freq_antennas[j];

          int x_i_diff = antenna_i.x - antenna_j.x;
          int y_i_diff = antenna_i.y - antenna_j.y;
          int new_x_i = antenna_i.x + x_i_diff;
          int new_y_i = antenna_i.y + y_i_diff;
          auto antinode_i_pos = std::pair<int, int>{new_x_i, new_y_i};
          if (new_x_i >= 0 && new_x_i < map_x && new_y_i >= 0 &&
              new_y_i < map_y && !unique_locations.count(antinode_i_pos)) {
            unique_locations.insert(std::move(antinode_i_pos));
          }

          int x_j_diff = antenna_j.x - antenna_i.x;
          int y_j_diff = antenna_j.y - antenna_i.y;
          int new_x_j = antenna_j.x + x_j_diff;
          int new_y_j = antenna_j.y + y_j_diff;
          auto antinode_j_pos = std::pair<int, int>{new_x_j, new_y_j};
          if (new_x_j >= 0 && new_x_j < map_x && new_y_j >= 0 &&
              new_y_j < map_y && !unique_locations.count(antinode_j_pos)) {
            unique_locations.insert(std::move(antinode_j_pos));
          }
        }
      }
    }
  }

  return unique_locations.size();
}

int count_unique_locations_for_antinode_with_resonant_harmonics(
    const std::map<char, std::vector<Antenna>> &antennas, int map_x,
    int map_y) {
  std::set<std::pair<int, int>> unique_locations;

  for (const auto &[frequency, same_freq_antennas] : antennas) {
    if (same_freq_antennas.size() > 1) {
      for (std::size_t i = 0; i < same_freq_antennas.size() - 1; ++i) {
        const auto &antenna_i = same_freq_antennas[i];
        auto antenna_i_pos = std::pair<int, int>{antenna_i.x, antenna_i.y};
        if (!unique_locations.count(antenna_i_pos)) {
          unique_locations.insert(std::move(antenna_i_pos));
        }

        for (std::size_t j = i + 1; j < same_freq_antennas.size(); ++j) {
          const auto &antenna_j = same_freq_antennas[j];
          auto antenna_j_pos = std::pair<int, int>{antenna_j.x, antenna_j.y};
          if (!unique_locations.count(antenna_j_pos)) {
            unique_locations.insert(std::move(antenna_j_pos));
          }

          int x_i_diff = antenna_i.x - antenna_j.x;
          int y_i_diff = antenna_i.y - antenna_j.y;
          int new_x_i = antenna_i.x + x_i_diff;
          int new_y_i = antenna_i.y + y_i_diff;
          while (new_x_i >= 0 && new_x_i < map_x && new_y_i >= 0 &&
                 new_y_i < map_y) {
            auto antinode_i_pos = std::pair<int, int>{new_x_i, new_y_i};
            if (!unique_locations.count(antinode_i_pos)) {
              unique_locations.insert(std::move(antinode_i_pos));
            }
            new_x_i += x_i_diff;
            new_y_i += y_i_diff;
          }

          int x_j_diff = antenna_j.x - antenna_i.x;
          int y_j_diff = antenna_j.y - antenna_i.y;
          int new_x_j = antenna_j.x + x_j_diff;
          int new_y_j = antenna_j.y + y_j_diff;
          while (new_x_j >= 0 && new_x_j < map_x && new_y_j >= 0 &&
                 new_y_j < map_y) {
            auto antinode_j_pos = std::pair<int, int>{new_x_j, new_y_j};
            if (!unique_locations.count(antinode_j_pos)) {
              unique_locations.insert(std::move(antinode_j_pos));
            }
            new_x_j += x_j_diff;
            new_y_j += y_j_diff;
          }
        }
      }
    }
  }

  return unique_locations.size();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "No input file provided!\n";
    return -1;
  }

  auto current_path = std::filesystem::current_path();
  auto path = std::filesystem::path{argv[1]};
  auto file_path = std::filesystem::canonical(current_path / path);

  auto f = std::ifstream{file_path};
  if (!f.is_open()) {
    std::cerr << "Can't open the file!\n";
    return -1;
  }

  std::string line;
  int y = 0;
  int x = 0;
  std::map<char, std::vector<Antenna>> antennas;

  while (std::getline(f, line)) {
    std::stringstream ss{line};
    x = 0;
    char sign;

    while (ss >> sign) {
      if (sign != '.') {
        auto a = Antenna{sign, x, y};
        if (antennas.count(sign)) {
          antennas[sign].push_back(std::move(a));
        } else {
          antennas[sign] = std::vector<Antenna>{std::move(a)};
        }
      }
      x++;
    }

    y++;
  }

  int locations_number = count_unique_locations_for_antinode(antennas, x, y);
  std::cout << "Unique locations for antinode: " << locations_number
            << std::endl;
  locations_number =
      count_unique_locations_for_antinode_with_resonant_harmonics(antennas, x,
                                                                  y);
  std::cout << "Unique locations for antinode with resonant harmonics: "
            << locations_number << std::endl;

  return 0;
}
