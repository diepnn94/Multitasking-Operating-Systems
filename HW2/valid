#!/bin/bash

# valid returns yes if the argument is a valid shell variable, else return no
# An argument is valid if it starts with alphabetical characters, both upper and lower, or and underscore
# And the body of the argument can only contain alphabetical characters, both upper and lower, integers, and underscore


if [[ $1 =~ ^[a-zA-Z|_][a-zA-Z0-9|_]*$ ]] ; 
then
	echo yes
else
	echo no
fi

