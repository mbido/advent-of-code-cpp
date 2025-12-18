# Advent of Code - High-Performance C++ Template

This repository contains my personal C++ setup for solving [Advent of Code](https://adventofcode.com/) challenges with a focus on extreme performance. It uses memory-mapped files (`mmap`) for lightning-fast I/O and custom parsing utilities to squeeze every bit of speed out of each puzzle.

## Project Structure

- `src/<year>/day<xx>/`: Where the magic happens (source code).
- `include/utils.hpp`: The "fast engine" – includes `mmap` wrappers, high-speed parsing, and benchmarking tools.
- `data/<year>/`: Your puzzle inputs (`dayxx.txt`) and examples (`testxx.txt`).
- `template/`: The boilerplate used to kickstart each new day.

## Getting Started

### 1. Initialize a New Day
To scaffold a new challenge, just run the helper script with the year and day:
```bash
./new_day.sh 2025 01
```
This creates your source file in `src/2025/day01/` and sets up empty data files in `data/2025/`.

### 2. Build & Run
The `run.sh` script is your swiss-army knife. It handles CMake configuration, compilation, and execution in one go:

```bash
# Run the solution on real input
./run.sh 2025 01

# Run using the test/example input
./run.sh 2025 01 -test

# Run without the benchmark (single execution)
./run.sh 2025 01 -nobench
```

## Tips
- **Testing**: Toggle between real data and examples using the `-test` flag – no need to change your code.
- **IntelliSense**: The project generates `compile_commands.json` automatically, so VS Code should provide perfect autocompletion and error checking out of the box.

Happy coding and may your solutions run in sub-microsecond time! 🚀
