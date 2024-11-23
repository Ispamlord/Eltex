#!/bin/bash
date=$(date +%T)
counter=$(grep -c "time" "Text.txt")
if [ $counter -eq 0 ]; then
counter=1
else
counter=$((counter+1))
fi
text_to_add="I run $counter time"
echo "$date $text_to_add" >> "Text.txt"
