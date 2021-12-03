#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)
	

def determineMostCommonValue(listOfBinaryStrings, indexPosition, tieWinner):
	zeros = 0
	ones = 0
	for eachString in listOfBinaryStrings:
		if ( eachString[indexPosition] == '1'):
			ones += 1
		else:
			zeros += 1
			
	if (ones > zeros):
		return 1
	elif (ones < zeros):
		return 0
	else:
		return tieWinner

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	diagReports = f.read().strip().split("\n")
	f.close()

	# Create gamma and epsilon strings
	gamma = ''
	epsilon = ''
	for i in range(0, len(diagReports[0])):
		if (determineMostCommonValue(diagReports, i, 0)):
			gamma += '1'
			epsilon += '0'
		else:
			gamma += '0'
			epsilon += '1'

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
