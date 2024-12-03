#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool is_safe_report(const std::vector<int> &levels, bool should_increase,
                    bool should_decrease) {
  int diff = 0;
  bool is_safe = true;

  for (int i = 1; i < levels.size(); ++i) {
    diff = levels[i - 1] - levels[i];

    if (std::abs(diff) < 1 || std::abs(diff) > 3) {
      is_safe = false;
      break;
    } else if (should_increase && diff >= 0) {
      is_safe = false;
      break;
    } else if (should_decrease && diff <= 0) {
      is_safe = false;
      break;
    }
  }

  return is_safe;
}

bool is_safe_report_after_applying_problem_dampener(
    const std::vector<int> &levels) {
  int init_diff = levels[0] - levels[1];
  bool should_increase = init_diff < 0;
  bool should_decrease = init_diff > 0;

  if (is_safe_report(levels, should_increase, should_decrease)) {
    return true;
  } else {
    for (int i = 0; i < levels.size(); ++i) {
      std::vector<int> levels_copy = levels;
      levels_copy.erase(levels_copy.begin() + i);

      if (levels_copy.size() < 2) {
        return true;
      }

      init_diff = levels_copy[0] - levels_copy[1];
      should_increase = init_diff < 0;
      should_decrease = init_diff > 0;

      if (is_safe_report(levels_copy, should_increase, should_decrease)) {
        return true;
      }
    }
  }

  return false;
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

  std::string report = "";
  std::vector<int> levels;
  int safe_reports = 0;

  while (std::getline(f, report)) {
    int level = 0;
    std::stringstream report_stream{report};

    while (report_stream >> level) {
      levels.push_back(level);
    }

    if (levels.size() < 2) {
      levels.clear();
      continue;
    }

    if (is_safe_report_after_applying_problem_dampener(levels)) {
      safe_reports++;
    }

    levels.clear();
  }

  std::cout << "Safe reports: " << safe_reports << std::endl;

  return 0;
}