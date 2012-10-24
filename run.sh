#!/bin/bash

#clear the output file
cat /dev/null > output.txt

#run BAPS for every line in the input file
file=$1
cat $file | while read line 
do
	echo "Executing $line"
	BAPS/bin/BAPS $line >> output.txt
done

