#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

void print_array(std::array<uint8_t, 100> a) {
  std::cout << "[ ";
  for (uint16_t i = 0; i < 100; i++) {
    std::cout << (int)a[i] << " ";
  }
  std::cout << "]" << std::endl;
}

void solve(const char *filename, bool silent = false) {
  aoc::MappedFile input(filename);
  const char *p = input.begin();
  const char *end = input.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  uint8_t joltage[12];

  /*
  option 1: -> 12 * length(numbers)
    prev = 0
    pour tout i de 0 à 11:
      joltage[i] = max(numbers[prev -> length + i - 11])
      prev = numbers.index(joltage[i])
  */
  std::array<uint8_t, 100> numbers; // never more than 100 elements.

  while (p < end) {

    if (*p == '\n') {
      p++;
    }

    uint16_t length = 0;
    for (uint16_t i = 0; p < end && *p != '\n'; i++, p++, length++) {
      numbers[i] = *p - '0';
    }

    // part1
    uint8_t a = 0, b = 0;
    for (uint16_t i = 0; i < length; i++) {
      uint8_t ni = numbers[i];
      if (i < length - 1 && ni > a) {
        a = ni;
        b = numbers[i + 1];
      } else if (ni > b) {
        b = ni;
      }
    }
    part1 += a * 10 + b;

    uint16_t prev = -1;
    for (uint16_t i = 0; i < 12; i++) {
      uint16_t idx_max = 0;
      joltage[i] = 0;
      for (uint16_t j = prev + 1; j < length + i - 11; j++) {
        if (numbers[j] > joltage[i]) {
          joltage[i] = numbers[j];
          idx_max = j;
        }
      }
      prev = idx_max;
    }

    uint64_t r = 0;
    for (uint8_t i = 0; i < 12; i++) {
      r = r * 10 + joltage[i];
    }
    part2 += r;
  }

  if (!silent) {
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
  }
}

int main(int argc, char **argv) {
  std::string filename = "data/2025/day03.txt";
  bool run_bench = true;
  bool silent = false;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      filename = "data/2025/test03.txt";
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
        std::cout << "Time: " << elapsed.count() << " μs\n";
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
// 17408
// 172740584266849