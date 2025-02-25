#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Error: You must provide both the file path and text string."
    exit 1
fi

writefile=$1
writestr=$2

dirpath=$(dirname "$writefile")
if [ ! -d "$dirpath" ]; then
    echo "Creating directory $dirpath"
    mkdir -p "$dirpath"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create directory $dirpath."
        exit 1
    fi
fi

echo "$writestr" > "$writefile"
if [ $? -ne 0 ]; then
    echo "Error: Failed to write to $writefile."
    exit 1
fi

echo "File $writefile has been created/overwritten with the provided content."

