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
mkdir -p "data/$YEAR"
cp template/main.cpp "$DIR/main.cpp"
touch "data/$YEAR/day$DAY.txt"
touch "data/$YEAR/test$DAY.txt"

# Update data paths in the new file
sed -i '' "s|data/dayXX.txt|data/$YEAR/day$DAY.txt|g" "$DIR/main.cpp"
sed -i '' "s|data/dayXX_test.txt|data/$YEAR/test$DAY.txt|g" "$DIR/main.cpp"

echo "Created $DIR and data/$YEAR/day$DAY.txt"
echo "You can now run: ./run.sh $YEAR $DAY"
