#include "utils.hpp"
#include "input.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <array>

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

// Fonction helper pour la Partie 2
// Calcule la somme des IDs invalides dans [start, end] pour une longueur FIXE
// 'len'
inline uint64_t solve_part2_range(uint64_t start, uint64_t end, uint16_t len) {
  if (len < 2)
    return 0;

  uint64_t range_total = 0;
  std::array<uint64_t, 10> prim_sums{};

  for (uint16_t L = 1; L <= len / 2; L++) {
    if (len % L != 0)
      continue;

    uint64_t M = get_multiplier(L, len);

    uint64_t P_min = (start + M - 1) / M;
    uint64_t P_max = end / M;

    if (P_min > P_max) {
      prim_sums[L] = 0;
      continue;
    }

    uint64_t sum_calc = (P_min + P_max) * (P_max - P_min + 1) / 2;
    uint64_t row_sum = sum_calc * M;

    for (uint16_t d = 1; d <= L / 2; d++) {
      if (L % d == 0) {
        row_sum -= prim_sums[d];
      }
    }

    prim_sums[L] = row_sum;
    range_total += row_sum;
  }

  return range_total;
}

void solve(std::string_view input_data, bool silent = false) {
  const char *p = input_data.begin();
  const char *end = input_data.end();

  uint64_t part1 = 0;
  uint64_t part2 = 0;

  while (p < end) {
    // Skip non-digits to handle newlines and commas robustly
    while (p < end && (*p < '0' || *p > '9') && *p != '-')
      p++;
    if (p >= end)
      break;

    uint64_t A = aoc::fast_atoi<uint64_t>(p);
    uint64_t a = A;

    while (p < end && *p != '-')
      p++;
    p++;

    uint64_t B = aoc::fast_atoi<uint64_t>(p);
    uint64_t b = B;

    uint64_t la = aoc::get_digit_count_32bit(a);
    uint64_t lb = aoc::get_digit_count_32bit(b);
    // we always have abs(la - lb) <= 1
    // if la % 2 == 1, a <- upper power of 10
    // if lb % 2 == 1, b <- lower power of 10

    if (la % 2 == 1) {
      a = aoc::power_of_10[la];
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
    if (xb >= a && xb <= b && xb != xa) {
      part1 += xb;
    }

    // adding invalides numbers in between
    for (int e = ad + 1; e < bd; e++) {
      uint64_t xe = e + e * mod;
      if (xe != xa && xe != xb && xe > a && xe < b) {
        part1 += xe;
      }
    }

    uint16_t lA = aoc::get_digit_count_32bit(A);
    uint16_t lB = aoc::get_digit_count_32bit(B);

    if (lA == lB) {
      part2 += solve_part2_range(A, B, lA);
    } else {
      uint64_t end_A = aoc::power_of_10[lA] - 1;
      part2 += solve_part2_range(A, end_A, lA);
      uint64_t start_B = aoc::power_of_10[lA];
      part2 += solve_part2_range(start_B, B, lB);
    }

    while (p < end && *p != ',')
      p++;
    p++;
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