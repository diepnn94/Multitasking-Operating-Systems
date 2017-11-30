#!/bin/bash

# lss return a sorted list of all file in the current directory in decreasing order of bytes
# lss does not take in any argument, so extra argument passed in will raise "No Argument Needed" message
# lss calls ls -l command to list out the file and their information
# Then calls sort command to sort in reverse order, sort by integer on column 5, which is the column of byte information
# Lastly sed is called to remove the last line of ls -l, which show the total bytes count in the current directory


if [ $# -eq 0 ]; 
then	
	ls -l |sed '1d' | sort -n -r -k5
else
	echo No Argument Needed
fi
