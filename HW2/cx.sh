#!/bin/bash

#This script will loop through the passed in arguments and perform chmod +x
#on each of the argument
#When one of the argument is invalid (does not exist), the chmod function will raise error
#and the script will continue loops through the rest of the argument

for i in "$@"
do
	chmod +x $i
done
