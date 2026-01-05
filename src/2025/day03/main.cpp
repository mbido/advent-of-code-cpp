#include "utils.hpp"
#include "input.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

inline const char *find_best_digit(const char *start, const char *end_limit) {
  if (start >= end_limit)
    return start;
  for (char d = '9'; d >= '0'; --d) {
    const char *p = (const char *)std::memchr(start, d, end_limit - start);
    if (p)
      return p;
  }
  return start;
}

void solve(std::string_view input_data, bool silent = false) {
  const char *p = input_data.begin();
  const char *end = input_data.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  while (p < end) {
    // Find end of line
    const char *eol = (const char *)std::memchr(p, '\n', end - p);
    if (!eol)
      eol = end;

    size_t len = eol - p;
    if (len == 0) {
      p = eol + 1;
      continue;
    }

    // Part 1
    uint8_t a = 0;
    const char *start_b = p;

    if (len >= 2) {
      const char *end_a = p + len - 1;
      const char *p_a = find_best_digit(p, end_a);
      a = *p_a - '0';
      start_b = p_a + 1;
    }

    uint8_t b = 0;
    if (start_b < p + len) {
      const char *p_b = find_best_digit(start_b, p + len);
      b = *p_b - '0';
    }
    part1 += a * 10 + b;

    const char *prev = p - 1;
    uint64_t r = 0;

    for (int i = 0; i < 12; ++i) {
      const char *search_start = prev + 1;
      const char *search_end = p + len - 11 + i;

      const char *p_digit = find_best_digit(search_start, search_end);
      r = r * 10 + (*p_digit - '0');
      prev = p_digit;
    }
    part2 += r;

    p = eol + 1;
  }

  if (!silent) {
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
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
    std::cout << "--- Running on: " << (test_mode ? "TEST DATA" : "REAL DATA") << " ---\n";
    std::cout << "--- Results ---\n";
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