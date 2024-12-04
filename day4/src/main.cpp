#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int count_xmas_words_diagonally(const std::vector<std::vector<char>> &letters,
                                int i, int j, int column_size, int row_size) {
  int xmas_words = 0;

  // Search diagonally up & right
  if (i - 3 >= 0 && j + 3 < row_size && letters[i - 1][j + 1] == 'M' &&
      letters[i - 2][j + 2] == 'A' && letters[i - 3][j + 3] == 'S') {
    xmas_words++;
  }

  // Search diagonally down & right
  if (i + 3 < column_size && j + 3 < row_size && letters[i + 1][j + 1] == 'M' &&
      letters[i + 2][j + 2] == 'A' && letters[i + 3][j + 3] == 'S') {
    xmas_words++;
  }

  // Search diagonally down & left
  if (i + 3 < column_size && j - 3 >= 0 && letters[i + 1][j - 1] == 'M' &&
      letters[i + 2][j - 2] == 'A' && letters[i + 3][j - 3] == 'S') {
    xmas_words++;
  }

  // Search diagonally up & left
  if (i - 3 >= 0 && j - 3 >= 0 && letters[i - 1][j - 1] == 'M' &&
      letters[i - 2][j - 2] == 'A' && letters[i - 3][j - 3] == 'S') {
    xmas_words++;
  }

  return xmas_words;
}

int count_xmas_words_vertically(const std::vector<std::vector<char>> &letters,
                                int i, int j, int column_size, int row_size) {
  int xmas_words = 0;

  if (i + 3 < column_size && letters[i + 1][j] == 'M' &&
      letters[i + 2][j] == 'A' && letters[i + 3][j] == 'S') {
    xmas_words++;
  }
  if (i - 3 >= 0 && letters[i - 1][j] == 'M' && letters[i - 2][j] == 'A' &&
      letters[i - 3][j] == 'S') {
    xmas_words++;
  }

  return xmas_words;
}

int count_xmas_words_horizontally(const std::vector<std::vector<char>> &letters,
                                  int i, int j, int column_size, int row_size) {
  int xmas_words = 0;

  if (j + 3 < row_size && letters[i][j + 1] == 'M' &&
      letters[i][j + 2] == 'A' && letters[i][j + 3] == 'S') {
    xmas_words++;
  }
  if (j - 3 >= 0 && letters[i][j - 1] == 'M' && letters[i][j - 2] == 'A' &&
      letters[i][j - 3] == 'S') {
    xmas_words++;
  }

  return xmas_words;
}

int count_xmas_words_in_all_directions(
    const std::vector<std::vector<char>> &letters, int i, int j,
    int column_size, int row_size) {
  int xmas_words = 0;

  xmas_words +=
      count_xmas_words_horizontally(letters, i, j, column_size, row_size);
  xmas_words +=
      count_xmas_words_vertically(letters, i, j, column_size, row_size);
  xmas_words +=
      count_xmas_words_diagonally(letters, i, j, column_size, row_size);

  return xmas_words;
}

int count_xmas_words(const std::vector<std::vector<char>> &letters) {
  if (letters.empty()) {
    return 0;
  }

  int all_xmas_words = 0;
  const int col_size = letters.size();
  const int row_size = letters[0].size();

  for (int i = 0; i < col_size; ++i) {
    for (int j = 0; j < row_size; ++j) {
      if (letters[i][j] == 'X') {
        all_xmas_words += count_xmas_words_in_all_directions(
            letters, i, j, col_size, row_size);
      }
    }
  }

  return all_xmas_words;
}

bool is_x_mas_shape(const std::vector<std::vector<char>> &letters, int i,
                    int j) {
  if ((letters[i - 1][j - 1] == 'M' && letters[i + 1][j + 1] == 'S') ||
      (letters[i - 1][j - 1] == 'S' && letters[i + 1][j + 1] == 'M')) {
    return (letters[i - 1][j + 1] == 'M' && letters[i + 1][j - 1] == 'S') ||
           (letters[i - 1][j + 1] == 'S' && letters[i + 1][j - 1] == 'M');
  }

  return false;
}

bool is_mas_in_x_shape(const std::vector<std::vector<char>> &letters, int i,
                       int j, int column_size, int row_size) {
  if (i - 1 >= 0 && i + 1 < column_size && j - 1 >= 0 && j + 1 < row_size) {
    return is_x_mas_shape(letters, i, j);
  }

  return false;
}

int count_xmas_shapes(const std::vector<std::vector<char>> &letters) {
  if (letters.empty()) {
    return 0;
  }

  int all_xmas_shapes = 0;
  const int col_size = letters.size();
  const int row_size = letters[0].size();

  for (int i = 0; i < col_size; ++i) {
    for (int j = 0; j < row_size; ++j) {
      if (letters[i][j] == 'A' &&
          is_mas_in_x_shape(letters, i, j, col_size, row_size)) {
        all_xmas_shapes++;
      }
    }
  }

  return all_xmas_shapes;
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

  std::vector<std::vector<char>> letters;
  std::string line;

  while (std::getline(f, line)) {
    std::stringstream ss{line};
    std::vector<char> row;
    row.reserve(line.size());
    char letter;

    while (ss >> letter) {
      row.push_back(letter);
    }

    letters.emplace_back(std::move(row));
  }

  // That's for part 1
  int xmas_words = count_xmas_words(letters);
  std::cout << "XMAS word appears " << xmas_words << " times." << std::endl;

  // That's for part 2
  int xmas_shapes = count_xmas_shapes(letters);
  std::cout << "X-MAS shape appears " << xmas_shapes << " times." << std::endl;

  return 0;
}