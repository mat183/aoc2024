#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

bool is_true_equation(const std::vector<int> &values, long long target_result,
                      long long current_result, std::size_t idx) {
  if (idx == values.size()) {
    return current_result == target_result;
  }

  return is_true_equation(values, target_result, current_result + values[idx],
                          idx + 1) ||
         is_true_equation(values, target_result, current_result * values[idx],
                          idx + 1) ||
         is_true_equation(values, target_result,
                          std::stoll(std::to_string(current_result) +
                                     std::to_string(values[idx])),
                          idx + 1);
}

long long count_total_calibration_result(
    const std::multimap<long long, std::vector<int>> &equations) {
  long long calibration_result = 0;

  for (const auto &[result, values] : equations) {
    std::size_t idx = 0;
    long long current_result = values[idx];
    if (is_true_equation(values, result, current_result, idx + 1)) {
      calibration_result += result;
    }
  }

  return calibration_result;
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

  std::regex re{R"(\d+)"};

  std::string line;

  std::multimap<long long, std::vector<int>> equations;

  while (std::getline(f, line)) {
    std::sregex_iterator it(line.begin(), line.end(), re);
    std::sregex_iterator end;

    long long equation_result = -1;
    std::vector<int> numbers;
    for (; it != end; ++it) {
      if (equation_result == -1) {
        equation_result = std::stoll(it->str());
      } else {
        numbers.push_back(std::stoi(it->str()));
      }
    }
    equations.insert({equation_result, numbers});
  }

  long long result = count_total_calibration_result(equations);

  std::cout << "Total calibration result: " << result << std::endl;

  return 0;
}