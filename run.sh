#!/bin/bash

if [ "$#" -lt 2 ]; then
    echo "Usage: ./run.sh <year> <day> [args...]"
    echo "Example: ./run.sh 2025 01 -test -nobench"
    exit 1
fi

YEAR=$1
DAY=$2
shift 2
# $@ contient maintenant tous les arguments restants (-test, -nobench, etc.)

EXE="aoc_${YEAR}_${DAY}"

# 1. Configuration et Compilation
mkdir -p build
cd build || exit
cmake -DCMAKE_BUILD_TYPE=Release .. > /dev/null
make "$EXE" -j

if [ $? -ne 0 ]; then
    echo "Error: Compilation failed for $EXE"
    exit 1
fi

# 2. Exécution depuis la racine
cd ..
./build/"$EXE" "$@"