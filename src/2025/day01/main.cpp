#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

void solve(const char *filename, bool silent = false) {
  aoc::MappedFile input(filename);
  const char *p = input.begin();
  const char *end = input.end();

  long long part1 = 0;
  long long part2 = 0;
  int current_pos = 50;

  while (p < end) {
    while (p < end && *p != 'L' && *p != 'R')
      p++;
    if (p >= end)
      break;

    char dir = *p++;
    long long dist = aoc::fast_atoi<long long>(p);

    if (dir == 'L') {
      int dist_to_zero = (current_pos == 0) ? 100 : current_pos;
      if (dist >= dist_to_zero) {
        part2 += (dist - dist_to_zero) / 100 + 1;
      }
      current_pos = (current_pos - (dist % 100) + 100) % 100;
    } else {
      int dist_to_zero = (current_pos == 0) ? 100 : (100 - current_pos);
      if (dist >= dist_to_zero) {
        part2 += (dist - dist_to_zero) / 100 + 1;
      }
      current_pos = (current_pos + dist) % 100;
    }

    if (current_pos == 0) {
      part1++;
    }
  }

  if (!silent) {
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
  }
}

int main(int argc, char **argv) {
  std::string filename = "data/2025/day01.txt";
  bool run_bench = true;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      filename = "data/2025/test01.txt";
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