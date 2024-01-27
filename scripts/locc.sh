#!/bin/bash

directoryPath="../Source"
totalLoc=0

files=$(find "$directoryPath" -type f \( -name "*.h" -o -name "*.cpp" \))

for file in $files; do
    loc=$(wc -l < "$file")
    echo "File: $file, LOC: $loc"
    totalLoc=$((totalLoc + loc))
done

echo "Total LOC in folder: $totalLoc"