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
  bool silent = false;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      filename = "data/dayXX_test.txt";
    } else if (arg == "-nobench") {
      run_bench = false;
    } else if (arg == "-silent") {
      silent = true;
    }
  }

  if (!silent) {
    std::cout << "--- Running on: " << filename << " ---\n";
    std::cout << "--- Results ---\n";
  }

  try {
    auto start = std::chrono::high_resolution_clock::now();
    solve(filename.c_str(), silent);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> elapsed = end - start;

    if (!silent) {
      if (run_bench) {
        aoc::run_benchmark([&](bool s) { solve(filename.c_str(), s); }, 1000);
      } else {
        std::cout << "Time: " << elapsed.count() << " us\n";
      }
    } else {
      if (run_bench) {
        auto res = aoc::measure_benchmark(
            [&](bool s) { solve(filename.c_str(), s); }, 1000);
        std::cout << res.best_time;
      } else {
        std::cout << elapsed.count();
      }
    }
  } catch (...) {
    if (!silent)
      std::cerr << "Execution failed\n";
    return 1;
  }

  return 0;
}