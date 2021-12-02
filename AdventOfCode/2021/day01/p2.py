#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def sumList(ld):
	retVal = 0
	for x in ld:
		retVal += x
	return retVal

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	
	sonarData = f.read().strip().split("\n")
	
	increasingCounter = 0
	
	intSonarData = [int(x) for x in sonarData]
	
	for i in range(3,len(sonarData)):

		prevList = intSonarData[i - 3:i]
		curList = intSonarData[i - 2: i+1]
		
		prevPoint = sumList(prevList)
		curPoint = sumList(curList)
		
		eprint("For i = {}, prev sum = {}, cur sum = {}".format(i, prevPoint, curPoint))

		
		if (prevPoint < curPoint):
			increasingCounter += 1

	print("Num increasing data points: {}".format(increasingCounter))


if __name__ == "__main__":
	main(sys.argv)
