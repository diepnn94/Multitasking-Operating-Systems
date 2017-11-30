#!/bin/bash

#whoson returns the total number of ugrad users who are current logged in
#The script first finds out who is currently logged on by calling who -q
#Since who -q last line contains the number of users logged in, sed is called to remove this last line
#In order to sort, the user logged on list need to break into separate lines by calling tr, then uniq is called to remove duplicate
#After sorting the list and removing duplicates, each username will be passed to the command groups to determine user's group status
#Using grep to only keep user who is ugrad
#After filtering ugrad users, using awk to only print out the first column, which is the username
#Using tr put all newline together separates by space

groups $(who -q |sed "$ d" | tr " " "\n"| sort | uniq ) | grep ugrad | awk -F: '{print $1}'|tr "\n" " "
printf "\n"

