#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	
	sonarData = f.read().strip().split("\n")
	intSonarData = [ int(x) for x in sonarData]
	
	prevMeas = None
	increasingCounter = 0
	
	for curPoint in intSonarData:
		eprint(curPoint)
		
		if (prevMeas != None):
			
			if (curPoint > prevMeas):
				increasingCounter += 1
				
		prevMeas = curPoint
			
	print("Num increasing data points: {}".format(increasingCounter))
	 
	f.close()


if __name__ == "__main__":
	main(sys.argv)
