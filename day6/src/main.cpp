#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

enum class Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT,
};

class Guard {
public:
  Guard(int starting_x, int starting_y, int max_x, int max_y)
      : x{starting_x}, y{starting_y}, max_x{max_x}, max_y{max_y} {}

  void turn_right() {
    direction_index = (direction_index + 1) % directions.size();
  }

  std::pair<int, int> next_move() const {
    switch (directions[direction_index]) {
    case Direction::UP:
      return {x, y - 1};
    case Direction::RIGHT:
      return {x + 1, y};
    case Direction::DOWN:
      return {x, y + 1};
    case Direction::LEFT:
      return {x - 1, y};
    default:
      break;
    }

    return {x, y};
  }

  void move() {
    switch (directions[direction_index]) {
    case Direction::UP: {
      y--;
      break;
    }
    case Direction::RIGHT: {
      x++;
      break;
    }
    case Direction::DOWN: {
      y++;
      break;
    }
    case Direction::LEFT: {
      x--;
      break;
    }
    default:
      break;
    }
  }

  bool is_in_area() const { return x >= 0 && x < max_x && y >= 0 && y < max_y; }

  int get_x() const { return x; }
  int get_y() const { return y; }

private:
  int x;
  int y;
  int max_x;
  int max_y;
  int direction_index = 0;
  std::vector<Direction> directions = {Direction::UP, Direction::RIGHT,
                                       Direction::DOWN, Direction::LEFT};
};

struct Position {
  int x;
  int y;
};

bool operator==(const Position &lhs, const Position &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <> struct std::hash<Position> {
  std::size_t operator()(const Position &p) const {
    return std::hash<int>{}(p.x) ^ std::hash<int>{}(p.y);
  }
};

template <> struct std::hash<std::pair<Position, Position>> {
  std::size_t operator()(const std::pair<Position, Position> &pp) const {
    return std::hash<Position>{}(pp.first) ^ std::hash<Position>{}(pp.second);
  }
};

using MappedRow = std::vector<char>;
using MappedArea = std::vector<MappedRow>;

std::pair<int, bool> do_patrol(const MappedArea &area, int i, int j) {
  const int area_x_size = area[0].size();
  const int area_y_size = area.size();
  Guard guard{j, i, area_x_size, area_y_size};

  std::unordered_set<Position> distinct_positions;
  std::unordered_set<std::pair<Position, Position>> steps;

  bool looped = false;

  while (guard.is_in_area()) {
    char curr_sign = area[guard.get_y()][guard.get_x()];
    auto curr_pos = Position{guard.get_x(), guard.get_y()};

    if (!distinct_positions.count(curr_pos)) {
      distinct_positions.insert(curr_pos);
    }

    auto mv = guard.next_move();
    while (mv.first >= 0 && mv.first < area_x_size && mv.second >= 0 &&
           mv.second < area_y_size && area[mv.second][mv.first] == '#') {
      guard.turn_right();
      mv = guard.next_move();
    }

    guard.move();
    auto next_pos = Position{mv.first, mv.second};
    auto step = std::pair<Position, Position>{curr_pos, next_pos};

    if (steps.count(step)) {
      looped = true;
      break;
    } else {
      steps.insert(step);
    }
  }

  return {distinct_positions.size(), looped};
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

  MappedArea area;
  int starting_x = 0;
  int starting_y = 0;

  int y = 0;
  std::string line;
  while (std::getline(f, line)) {
    std::stringstream ss{line};

    int x = 0;
    MappedRow row;
    char pos;

    while (ss >> pos) {
      if (pos == '^') {
        starting_x = x;
        starting_y = y;
      }
      row.push_back(pos);
      x++;
    }

    area.push_back(std::move(row));
    y++;
  }

  auto single_patrol = do_patrol(area, starting_y, starting_x);
  std::cout << "Guard will visit " << single_patrol.first
            << " distinct positions before leaving the mapped area."
            << std::endl;

  int positions_causing_loop = 0;
  for (int i = 0; i < area.size(); ++i) {
    for (int j = 0; j < area[0].size(); ++j) {
      if (area[i][j] == '.') {
        area[i][j] = '#';
        auto patrol = do_patrol(area, starting_y, starting_x);
        if (patrol.second) {
          positions_causing_loop++;
        }
        area[i][j] = '.';
      }
    }
  }

  std::cout << "There are " << positions_causing_loop
            << " different positions that cause loop for the guard."
            << std::endl;

  return 0;
}