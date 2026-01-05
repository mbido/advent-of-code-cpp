#include "utils.hpp"
#include "input.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>

void solve(std::string_view input_data, bool silent = false) {
  const char *p = input_data.begin();
  const char *end = input_data.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  // Parsing logic here using p and end
  // ...

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
    std::cout << "--- Running on: " << (test_mode ? "TEST DATA" : "REAL DATA") << " ---
";
    std::cout << "--- Results ---
";
  }

  try {
    auto start = std::chrono::high_resolution_clock::now();
    solve(input, silent);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> elapsed = end - start;

    if (!silent) {
      if (run_bench) {
        aoc::run_benchmark([&](bool s) { solve(input, s); }, 1000);
      } else {
        std::cout << "Time: " << elapsed.count() << " μs\n";
      }
    } else {
      if (run_bench) {
        auto res = aoc::measure_benchmark(
            [&](bool s) { solve(input, s); }, 1000);
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
