#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error: You must provide both the directory and search string."
    exit 1
fi

filesdir=$1
searchstr=$2

if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a valid directory."
    exit 1
fi

file_count=0
line_count=0

while IFS= read -r file; do
    ((file_count++))

    match_lines=$(grep -c "$searchstr" "$file")
    ((line_count += match_lines))
done < <(find "$filesdir" -type f)

echo "The number of files are $file_count and the number of matching lines are $line_count"

