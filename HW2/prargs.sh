#!/bin/bash

# prargs prints out a number list of arguments in the order they were passed in
# using a for loop starting at index 0 to print out the number line
# prints out the current position of the argument starting at 0, and the argument value

for i in $(seq 0 $#)
do
	echo "$i: \"${!i}\""
done

