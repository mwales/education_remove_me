#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def sumList(ld):
	retVal = 0
	for x in ld:
		retVal += int(x)
	return retVal

def main():
	f = open(sys.argv[1])
	
	sonarData = f.read().strip().split("\n")
	
	increasingCounter = 0
	
	for i in range(3,len(sonarData)):

		prevList = sonarData[i - 3:i]
		curList = sonarData[i - 2: i+1]
		
		prevPoint = sumList(prevList)
		curPoint = sumList(curList)
		
		eprint("For i = {}, prev sum = {}, cur sum = {}".format(i, prevPoint, curPoint))

		
		if (prevPoint < curPoint):
			increasingCounter += 1

	print("Num increasing data points: {}".format(increasingCounter))


if __name__ == "__main__":
	main()
