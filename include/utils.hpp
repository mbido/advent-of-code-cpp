#pragma once

#include <charconv>
#include <chrono>
#include <fcntl.h>
#include <fmt/format.h>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

namespace aoc {

// ---- math helper ----

std::vector<uint64_t> power_of_10 = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
    10000000000000,
    100000000000000,
    1000000000000000,
    10000000000000000,
    100000000000000000,
    1000000000000000000,
};

template <std::integral T> constexpr T power(T base, long long exp) {
  T res = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      res *= base;
    base *= base;
    exp /= 2;
  }
  return res;
}

inline int get_digit_count_32bit(uint32_t v) {
  if (v < 10000) {
    if (v < 100)
      return (v < 10) ? 1 : 2;
    return (v < 1000) ? 3 : 4;
  }
  if (v < 100000000) {
    if (v < 1000000)
      return (v < 100000) ? 5 : 6;
    return (v < 10000000) ? 7 : 8;
  }
  return (v < 1000000000) ? 9 : 10;
}
// ---------------------

class MappedFile {
public:
  MappedFile(const char *filename) {
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
      perror("Error opening file");
      exit(1);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
      perror("Error getting file size");
      exit(1);
    }
    file_size = sb.st_size;

    data = static_cast<const char *>(
        mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED) {
      perror("Error mapping file");
      exit(1);
    }

    // Advise the kernel we will read sequentially
    madvise((void *)data, file_size, MADV_SEQUENTIAL);
  }

  ~MappedFile() {
    if (data != MAP_FAILED)
      munmap((void *)data, file_size);
    if (fd != -1)
      close(fd);
  }

  std::string_view view() const { return {data, file_size}; }

  const char *begin() const { return data; }
  const char *end() const { return data + file_size; }
  size_t size() const { return file_size; }

private:
  int fd = -1;
  const char *data = (const char *)MAP_FAILED;
  size_t file_size = 0;
};

// Fast integer parsing
template <typename T> inline T fast_atoi(const char *&p) {
  T val = 0;
  bool neg = false;
  if (*p == '-') {
    neg = true;
    ++p;
  }
  while (*p >= '0' && *p <= '9') {
    val = val * 10 + (*p - '0');
    ++p;
  }
  return neg ? -val : val;
}

inline void skip_non_digits(const char *&p) {
  while (*p && (*p < '0' || *p > '9') && *p != '-')
    ++p;
}

// Prevents compiler from optimizing away unused variables
template <class T>
inline void DoNotOptimize(T const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

struct BenchmarkResult {
  double average_time;
};

template <typename Func>
BenchmarkResult measure_benchmark(Func &&func, int iterations = 1000) {
  // Warmup
  for (int i = 0; i < 10; ++i) {
    func(true);
  }

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; ++i) {
    func(true);
  }

  auto end = std::chrono::high_resolution_clock::now();
  // On calcule d'abord en nanosecondes (entier 64 bits) pour la précision brute
  auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  
  // Conversion finale en double (64 bits) pour la moyenne en microsecondes
  return {static_cast<double>(total_ns) / (iterations * 1000.0)};
}

template <typename Func>
void run_benchmark(Func &&func, int iterations = 1000) {
  BenchmarkResult res = measure_benchmark(std::forward<Func>(func), iterations);

  std::cout << "\n--- Benchmark (" << iterations << " iterations) ---\n";
  std::cout << "Average:   " << std::fixed << std::setprecision(6) << res.average_time << " μs\n";
  std::cout << "-----------------------------------\n";
}

} // namespace aoc