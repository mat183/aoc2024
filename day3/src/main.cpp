#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

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
  std::regex pattern{R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))"};
  long long result = 0;
  bool multiplications_enabled = true;

  const std::string do_switch = "do()";
  const std::string dont_switch = "don't()";

  while (std::getline(f, line)) {
    auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), pattern);
    auto matchesEnd = std::sregex_iterator();

    for (auto it = matchesBegin; it != matchesEnd; ++it) {
      std::smatch match = *it;

      if (match.str(0) == do_switch) {
        multiplications_enabled = true;
      } else if (match.str(0) == dont_switch) {
        multiplications_enabled = false;
      } else if (match.size() == 3 && multiplications_enabled) {
        long long num1 = std::stoll(match.str(1));
        long long num2 = std::stoll(match.str(2));
        result += num1 * num2;
      }
    }
  }

  std::cout << "Result of all enabled multiplications: " << result << std::endl;

  return 0;
}