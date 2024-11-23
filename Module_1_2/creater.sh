#!/bin/bash

for ((j=1;j<4;j++))
do
	mkdir "folder_$j"
	for((f=1;f<4;f++))
	do
	mkdir "folder_$j/subfolder_$f"
		for((i=1;i<=4;i++))
		do
			touch "folder_$j/subfolder_$f/empty_$i.txt"
		done
	done
done
