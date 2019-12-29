#!/bin/bash

g++ ElfComputer.cpp ArcadeScreen.cpp main.cpp -o pt1 -l ncurses
g++ -g ElfComputer.cpp ArcadeScreen.cpp main2.cpp -o pt2 -l ncurses

