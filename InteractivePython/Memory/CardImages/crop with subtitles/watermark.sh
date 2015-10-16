#!/bin/bash

convert $1  -font Helvetica -pointsize 28 \
          -draw "gravity south \
                 fill black  text 0,12 '$2' \
                 fill white  text 1,11 '$2' " \
          $1.sub.jpg
