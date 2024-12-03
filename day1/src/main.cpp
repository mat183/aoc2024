#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

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

  std::multiset<int> left_nums;
  std::map<int, int> right_nums_map;
  int similarity_score = 0;
  int counter = 0;
  int number = 0;

  while (f >> number) {
    if (counter % 2 == 0) {
      left_nums.insert(number);
    } else {
      if (right_nums_map.count(number)) {
        right_nums_map[number]++;
      } else {
        right_nums_map[number] = 1;
      }
    }

    counter++;
  }

  for (const auto num : left_nums) {
    if (right_nums_map.count(num)) {
      similarity_score += num * right_nums_map[num];
    }
  }

  std::cout << "Similarity score: " << similarity_score << std::endl;

  return 0;
}