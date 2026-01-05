#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: ./new_day.sh <year> <day_number>"
    echo "Example: ./new_day.sh 2025 01"
    exit 1
fi

YEAR=$1
DAY=$2
DIR="src/$YEAR/day$DAY"

if [ -d "$DIR" ]; then
    echo "Error: $DIR already exists."
    exit 1
fi

mkdir -p "$DIR"
cp template/main.cpp "$DIR/main.cpp"
cp template/input.hpp "$DIR/input.hpp"

echo "Created $DIR with main.cpp and input.hpp"
echo "You can now run: ./run.sh $YEAR $DAY"