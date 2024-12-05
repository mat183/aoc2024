#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

void fix_incorrectly_ordered_update(const std::set<std::string> &ordering_rules,
                                    std::vector<int> &update) {
  std::string oposite_rule;
  bool incorrect_order_found = false;

  for (int i = 0; i < update.size() - 1;) {
    for (int j = i + 1; j < update.size(); ++j) {
      oposite_rule =
          std::to_string(update[j]) + "|" + std::to_string(update[i]);
      if (ordering_rules.count(oposite_rule)) {
        incorrect_order_found = true;
        std::swap(update[i], update[j]);
        break;
      }
    }

    if (incorrect_order_found) {
      incorrect_order_found = false;
      continue;
    }

    i++;
  }
}

bool is_update_correctly_ordered(const std::set<std::string> &ordering_rules,
                                 const std::vector<int> &update) {
  std::string oposite_rule;

  for (int i = 0; i < update.size() - 1; ++i) {
    for (int j = i + 1; j < update.size(); ++j) {
      oposite_rule =
          std::to_string(update[j]) + "|" + std::to_string(update[i]);
      if (ordering_rules.count(oposite_rule)) {
        return false;
      }
    }
  }

  return true;
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

  bool reading_page_ordering_rules = true;
  std::string line;

  std::set<std::string> ordering_rules;
  int middle_page_sum = 0;
  int fixed_updates_middle_page_sum = 0;

  while (std::getline(f, line)) {
    if (line.empty()) {
      reading_page_ordering_rules = false;
    }

    if (reading_page_ordering_rules) {
      ordering_rules.insert(line);
    } else {
      if (!line.empty()) {
        std::stringstream update_stream{line};
        std::vector<int> update;
        std::string page;

        while (std::getline(update_stream, page, ',')) {
          update.push_back(std::stoi(page));
        }

        if (is_update_correctly_ordered(ordering_rules, update)) {
          middle_page_sum += update[update.size() / 2];
        } else {
          fix_incorrectly_ordered_update(ordering_rules, update);
          fixed_updates_middle_page_sum += update[update.size() / 2];
        }
      }
    }
  }

  std::cout << "Middle page number sum of correctly ordered updates: "
            << middle_page_sum << std::endl;
  std::cout
      << "Middle page number sum of incorrectly ordered (but fixed) updates: "
      << fixed_updates_middle_page_sum << std::endl;

  return 0;
}