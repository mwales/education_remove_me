#!/bin/bash

echo "Compiling my solution:"
clang++ --std=c++11 cj.cpp -o cj

echo "Compiling answer checker"
clang++ --std=c++11 cj-checker.cpp -o checker

