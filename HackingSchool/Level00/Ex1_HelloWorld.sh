#!/bin/bash

echo "We are going to make a directory with a file with your secret text: $1"

mkdir ex1
cd ex1
echo "$1" > secret.txt

echo "Done!"

