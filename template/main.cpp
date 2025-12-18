#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

void solve(const char *filename, bool silent = false) {
  aoc::MappedFile input(filename);
  const char *p = input.begin();
  const char *end = input.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  if (!silent) {
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
  }
}

int main(int argc, char **argv) {
  std::string filename = "data/dayXX.txt";
  bool run_bench = true;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      filename = "data/dayXX_test.txt";
    } else if (arg == "-nobench") {
      run_bench = false;
    }
  }

  std::cout << "--- Running on: " << filename << " ---\n";
  std::cout << "--- Results ---\n";
  solve(filename.c_str(), false);

  if (run_bench) {
    aoc::run_benchmark([&](bool s) { solve(filename.c_str(), s); }, 5000);
  }

  return 0;
}