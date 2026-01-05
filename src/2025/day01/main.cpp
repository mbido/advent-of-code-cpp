#include "input.hpp"
#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>

void solve(std::string_view input_data, bool silent = false) {
  const char *p = input_data.begin();
  const char *end = input_data.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;
  int current_pos = 50;

  while (p < end) {
    while (p < end && *p != 'L' && *p != 'R')
      p++;
    if (p >= end)
      break;

    char dir = *p++;
    uint64_t dist = aoc::fast_atoi<uint64_t>(p);

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
  } else {
    aoc::DoNotOptimize(part1);
    aoc::DoNotOptimize(part2);
  }
}

int main(int argc, char **argv) {
  bool run_bench = true;
  bool silent = false;
  bool test_mode = false;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      test_mode = true;
    } else if (arg == "-nobench") {
      run_bench = false;
    } else if (arg == "-silent") {
      silent = true;
    }
  }

  std::string_view input = test_mode ? Input::test_data : Input::data;

  if (!silent) {
    std::cout << "--- Running on: " << (test_mode ? "TEST DATA" : "REAL DATA")
              << " ---\n";
    std::cout << "--- Results ---\n";
  }

  try {
    auto start = std::chrono::high_resolution_clock::now();
    solve(input, silent);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> elapsed = end - start;

    if (!silent) {
      if (run_bench) {
        aoc::run_benchmark([&](bool s) { solve(input, s); }, 10000);
      } else {
        std::cout << "Time: " << elapsed.count() << " μs\n";
      }
    } else {
      if (run_bench) {
        auto res =
            aoc::measure_benchmark([&](bool s) { solve(input, s); }, 10000);
        std::cout << res.average_time;
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
