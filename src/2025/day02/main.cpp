#include "utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

int small_int_size(int i) {}

inline uint64_t get_multiplier(int L, int TotalLen) {
  uint64_t m = 0;

  int k = TotalLen / L;
  uint64_t shift = aoc::power_of_10[L];

  for (int i = 0; i < k; ++i) {
    m = m * shift + 1;
  }
  return m;
}

void solve(const char *filename, bool silent = false) {
  aoc::MappedFile input(filename);
  const char *p = input.begin();
  const char *end = input.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  while (p < end) {

    uint64_t a = aoc::fast_atoi<uint64_t>(p);

    while (p < end && *p != '-')
      p++;
    p++;

    uint64_t b = aoc::fast_atoi<uint64_t>(p);
    uint64_t la = aoc::get_digit_count_32bit(a);
    uint64_t lb = aoc::get_digit_count_32bit(b);
    // we always have abs(la - lb) <= 1
    // if la % 2 == 1, a <- upper power of 10
    // if lb % 2 == 1, b <- lower power of 10
    if (la % 2 == 1) {
      a = aoc::power_of_10[la + 1];
      la++;
    }
    if (lb % 2 == 1) {
      b = aoc::power_of_10[lb - 1];
      lb--;
    }

    // adding a and b if needed
    uint64_t mod = aoc::power(10, la / 2);
    uint64_t am = a % mod;
    uint64_t ad = a / mod;
    uint64_t bm = b % mod;
    uint64_t bd = b / mod;
    uint64_t xa = -1;
    uint64_t xb = -1;

    xa = ad * mod + ad;
    if (xa >= a && xa <= b) {
      part1 += xa;
    }

    xb = bd * mod + bd;
    if (xb >= a && xb <= b) {
      if (xb != xa) {
        part1 += xb;
      }
    }

    // adding invalides numbers in between
    for (int e = ad + 1; e < bd; e++) {
      uint64_t xe = e + e * mod;
      if (xe != xa && xe != xb && xe > a && xe < b) {
        part1 += xe;
      }
    }

    while (p < end && *p != ',')
      p++;
    p++;
  }

  if (!silent) {
    std::cout << "Part 1: " << part1 << "\n";
    std::cout << "Part 2: " << part2 << "\n";
  }
}

int main(int argc, char **argv) {
  std::string filename = "data/2025/day02.txt";
  bool run_bench = true;
  bool silent = false;

  for (int i = 1; i < argc; ++i) {
    std::string_view arg(argv[i]);
    if (arg == "-test") {
      filename = "data/2025/test02.txt";
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