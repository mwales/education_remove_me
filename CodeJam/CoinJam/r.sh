#!/bin/bash

cat $1 | ./cj > output_temp.txt

cat output_temp.txt

echo "Checking answer"

cat output_temp.txt | ./checker

