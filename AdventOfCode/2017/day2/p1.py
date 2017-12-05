#!/usr/bin/env python

# Alternate solution in python cause my C++ solution felt very clunky

import sys

def processSingleLine(text):
  numberList = text.split('\t')
  # print("!WS= {}".format(numberList))

  min = int(numberList[0])
  max = int(numberList[0])
  for numberString in numberList:
    curNumber = int(numberString)
    if (curNumber < min):
      min = curNumber
    if (curNumber > max):
      max = curNumber
  return max-min

def processFile(filename):
  checksum = 0
  with open(filename, 'r') as infile:
    for textLine in infile:
      # print("Read in: {}".format(textLine))
      checksum += processSingleLine(textLine)
      print("Checksum so far = {}".format(checksum))
  
  print("Final Checksum = {}".format(checksum))




def main(argv):

  if (len(argv) != 2):
    print("Usage: [python] {} filename".format(argv[0]))
    return

  processFile(argv[1])


if __name__ == "__main__":
  main(sys.argv)


