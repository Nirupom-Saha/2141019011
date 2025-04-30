#!/bin/bash

count_lines() {
	local file="$1"
	if [ -f "$file" ]; then
		lines=$(wc -l < "$file")
		echo "$file: $lines lines"
	else
		echo "$file: File not found"
	fi
}

file_list=("file1.txt" "file2.txt" "file3.txt")

for file in "${file_list[@]}"; do
	count_lines "$file"
done

