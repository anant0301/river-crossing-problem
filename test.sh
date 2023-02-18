#!/bin/bash

make schedule

while IFS= read -r line; do
    echo "Input: $line"
    echo "Output:"
    ./schedule.out $line

done < inputs.txt