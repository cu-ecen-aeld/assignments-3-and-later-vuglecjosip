#!/bin/sh
# finder.sh
# Usage: finder.sh <directory> <search-string>

if [ $# -ne 2 ]; then
    echo "Usage: $0 <directory> <search-string>"
    exit 1
fi

DIRECTORY=$1
SEARCH_STRING=$2

echo "Searching in directory: $DIRECTORY"
echo "Searching for string: $SEARCH_STRING"

if [ ! -d "$DIRECTORY" ]; then
    echo "Directory $DIRECTORY does not exist"
    exit 1
fi

NUM_FILES=$(find "$DIRECTORY" -type f | wc -l)
NUM_MATCHING_LINES=$(grep -r "$SEARCH_STRING" "$DIRECTORY" | wc -l)

echo "The number of files are $NUM_FILES and the number of matching lines are $NUM_MATCHING_LINES"

