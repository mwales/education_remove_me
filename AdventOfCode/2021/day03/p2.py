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
		
def trimList(listOfBinaryStrings, indexPosition, goodValue):
	retList = []
	for curString in listOfBinaryStrings:
		if (int(curString[indexPosition]) == goodValue):
			retList.append(curString)
	return retList

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	diagReports = f.read().strip().split("\n")
	f.close()

	o2List = diagReports
	co2List = diagReports
	
	# o2
	for i in range(0, len(diagReports[0])):
		cv = determineMostCommonValue(o2List, i, 1)
		o2List = trimList(o2List, i, cv)
		
		#eprint("O2 i={}, list={}".format(i, o2List))
		
		if (len(o2List) <= 1):
			break
	
	eprint("Len of o2List now = {}".format(len(o2List)))
	
	# co2
	for i in range(0, len(diagReports[0])):
		cv = determineMostCommonValue(co2List, i, 1)
		
		if (cv == 1):
			lcv = 0
		else:
			lcv = 1
		
		co2List = trimList(co2List, i, lcv)
		
		#eprint("CO2 i={}, list={}".format(i, co2List))
		
		if (len(co2List) <= 1):
			break
	
	eprint("Len of co2List now = {}".format(len(co2List)))
	
	if ( (len(o2List) != 1) or (len(co2List) != 1) ):
		eprint("p2 failure")
		return
		
	o2Rate = int(o2List[0], 2)
	co2Rate = int(co2List[0], 2)
	
	eprint("o2 = {}".format(o2Rate))
	eprint("co2 = {}".format(co2Rate))
	
	p2 = o2Rate * co2Rate
	
	print("p2 = {}".format(p2))
	

if __name__ == "__main__":
	main(sys.argv)
