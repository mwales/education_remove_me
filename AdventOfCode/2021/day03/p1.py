#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	diagReports = f.read().strip().split("\n")
	f.close()
	
	# Initiaze the counters list
	oneCounters = []
	zeroCounters = []
	firstReport = diagReports[0]
	for eachBit in firstReport:
		oneCounters.append(0)
		zeroCounters.append(0)
	
	
	for dr in diagReports:
		eprint(dr)
		
		# Iterate over the bits
		for i in range(0, len(dr)):
			curBit = dr[i]
			if (curBit == '1'):
				oneCounters[i] += 1
			else:
				zeroCounters[i] += 1
				
	# Create gamma and epsilon strings
	gamma = ''
	epsilon = ''
	for i in range(0, len(firstReport)):
		if (oneCounters[i] > zeroCounters[i]):
			gamma += '1'
			epsilon += '0'
		elif (oneCounters[i] < zeroCounters[i]):
			gamma += '0'
			epsilon += '1'
		else:
			eprint("For bit {}, the 1s == 0s == {}".format(i, oneCounters[i]))
			
	eprint("gamma = {}".format(gamma))
	eprint("epsilon = {}".format(epsilon))
	
	 
	gammaRate = int(gamma, 2)
	epsilonRate = int(epsilon, 2)
	
	eprint("gammaRate = {}".format(gammaRate))
	eprint("epsilonRate = {}".format(epsilonRate))
	
	p1 = gammaRate * epsilonRate
	print("P1 = {}".format(p1))

if __name__ == "__main__":
	main(sys.argv)
