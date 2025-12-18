#pragma once

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string_view>
#include <charconv>

namespace aoc {

class MappedFile {
public:
    MappedFile(const char* filename) {
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

        data = static_cast<const char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0));
        if (data == MAP_FAILED) {
            perror("Error mapping file");
            exit(1);
        }
        
        // Advise the kernel we will read sequentially
        madvise((void*)data, file_size, MADV_SEQUENTIAL);
    }

    ~MappedFile() {
        if (data != MAP_FAILED) munmap((void*)data, file_size);
        if (fd != -1) close(fd);
    }

    std::string_view view() const {
        return {data, file_size};
    }

    const char* begin() const { return data; }
    const char* end() const { return data + file_size; }
    size_t size() const { return file_size; }

private:
    int fd = -1;
    const char* data = (const char*)MAP_FAILED;
    size_t file_size = 0;
};

// Fast integer parsing
template<typename T>
inline T fast_atoi(const char*& p) {
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

inline void skip_non_digits(const char*& p) {
    while (*p && (*p < '0' || *p > '9') && *p != '-') ++p;
}

template<typename Func>
void run_benchmark(Func&& func, int iterations = 1000) {
    // Warmup
    for (int i = 0; i < 10; ++i) {
        func(true);
    }

    auto start = std::chrono::high_resolution_clock::now();
    double min_time = 1e18;
    
    for (int i = 0; i < iterations; ++i) {
        auto i_start = std::chrono::high_resolution_clock::now();
        func(true);
        auto i_end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::micro> i_elapsed = i_end - i_start;
        if (i_elapsed.count() < min_time) min_time = i_elapsed.count();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> total_elapsed = end - start;

    std::cout << "\n--- Benchmark (" << iterations << " iterations) ---\n";
    std::cout << "Best time: " << min_time << " us\n";
    std::cout << "Average:   " << total_elapsed.count() / iterations << " us\n";
    std::cout << "-----------------------------------\n";
}

} // namespace aoc
