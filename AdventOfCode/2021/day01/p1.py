#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def main():
	f = open(sys.argv[1])
	
	sonarData = f.read().strip().split("\n")
	
	prevMeas = None
	increasingCounter = 0
	
	for dataPoint in sonarData:
		eprint(dataPoint)
		
		curPoint = int(dataPoint)
		
		if (prevMeas != None):
			
			if (curPoint > prevMeas):
				increasingCounter += 1
				
		prevMeas = curPoint
			
	print("Num increasing data points: {}".format(increasingCounter))


if __name__ == "__main__":
	main()
