#!/bin/bash

groups $(who -q | sed "$ d" | tr " " "\n" |sort |uniq) | grep $(groups) | wc -l
