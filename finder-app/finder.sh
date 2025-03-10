#!/bin/sh

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

find "$filesdir" -type f | while IFS= read -r file; do
    # Increment file counter
    file_count=$((file_count + 1))

    # Ensure file exists before processing
    if [ ! -f "$file" ]; then
        echo "Skipping non-file: $file"
        continue
    fi

    # Count matches in the file
    match_lines=$(grep -c "$searchstr" "$file" 2>/dev/null)
    if [ $? -eq 0 ]; then
        line_count=$((line_count + match_lines))
    else
        echo "Error processing file: $file"
    fi
done


echo "The number of files are $file_count and the number of matching lines are $line_count"

