#!/bin/bash

# run_all.sh: Run all available AOC challenges and summarize execution times.
# Usage: 
#   ./run_all.sh          (Runs all years)
#   ./run_all.sh 2025     (Runs only 2025)

SELECTED_YEAR=$1

# 1. Compile everything
mkdir -p build
cd build || exit
echo "Configuring and compiling all targets..."
cmake -DCMAKE_BUILD_TYPE=Release .. > /dev/null
make -j > /dev/null
cd ..

echo "------------------------------------------------"
printf "% -10s | % -10s | % -15s\n" "Year" "Day" "Time (μs)"
echo "------------------------------------------------"

TOTAL_TIME=0
COUNT=0

# Find all years in src/
YEARS=$(ls src/ | grep -E '^[0-9]+$' | sort)

for YEAR in $YEARS; do
    if [ -n "$SELECTED_YEAR" ] && [ "$YEAR" != "$SELECTED_YEAR" ]; then
        continue
    fi

    # Find all days for this year
    if [ -d "src/$YEAR/" ]; then
        YEAR_TOTAL=0
        DAYS=$(ls "src/$YEAR/" | grep -E '^day[0-9]+$' | sort)

        for DAY_DIR in $DAYS; do
            DAY_NUM=${DAY_DIR#day}
            EXE="aoc_${YEAR}_${DAY_NUM}"

            if [ -f "build/$EXE" ]; then
                # Run in silent mode to get just the microseconds
                TIME=$(./build/"$EXE" -silent 2>/dev/null)
                EXIT_CODE=$?
                
                # Check if TIME is a valid number and exit code is 0
                if [ $EXIT_CODE -eq 0 ] && [[ $TIME =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
                    printf "% -10s | % -10s | % -15s\n" "$YEAR" "$DAY_NUM" "$TIME"
                    TOTAL_TIME=$(echo "$TOTAL_TIME + $TIME" | bc)
                    YEAR_TOTAL=$(echo "$YEAR_TOTAL + $TIME" | bc)
                    COUNT=$((COUNT + 1))
                else
                    printf "% -10s | % -10s | % -15s\n" "$YEAR" "$DAY_NUM" "Failed/Empty"
                fi
            fi
        done

        if (( $(echo "$YEAR_TOTAL > 0" | bc -l) )); then
             MS_YEAR_TIME=$(echo "scale=3; $YEAR_TOTAL / 1000" | bc -l)
             printf "% -10s | % -10s | % -15s\n" "" "" "----------"
             printf "% -10s | % -10s | %s\n" "Total $YEAR" "" "$YEAR_TOTAL μs ($MS_YEAR_TIME ms)"
             echo "------------------------------------------------"
        fi
    fi
done

echo ""
if [ "$COUNT" -gt 0 ]; then
    MS_TIME=$(echo "scale=3; $TOTAL_TIME / 1000" | bc -l)
    printf "Total challenges run: %d\n" "$COUNT"
    printf "Total execution time: %s μs (%s ms)\n" "$TOTAL_TIME" "$MS_TIME"
else
    echo "No challenges found to run."
fi
echo "------------------------------------------------"