#!/bin/bash

s1="Hello, world!"
i1=42

echo "$s1"

if [ "$i1" -gt 50 ]; then
	echo "The integer is greater than 50."
else
	echo "The integer is 50 or less."
fi

