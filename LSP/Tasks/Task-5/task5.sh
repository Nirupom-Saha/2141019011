#!/bin/bash

word="example"        
file="sample.txt"  

if [ ! -f "$file" ]; then
	echo "Error: File '$file' not found."
	exit 1
fi

grep -i "$word" "$file" | sort

