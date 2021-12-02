#!/usr/bin/env python3

import sys

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def main(argv):
	
	if (len(argv) < 2):
		print("Usage: {} inputfile".format(sys.argv[0]))
		return
	
	f = open(argv[1])
	
	subCommands = f.read().strip().split("\n")
	
	x = 0
	y = 0
	aim = 0
	
	for sc in subCommands:
		#eprint(sc)
		
		cmdParts = sc.split(' ')
		
		if (len(cmdParts) != 2):
			eprint("Bad command: {}".format(sc))
			continue
		
		val = int(cmdParts[1])
		if (cmdParts[0] == "forward"):
			x += val
			y += aim * val
		elif (cmdParts[0] == "down"):
			aim += val
		elif (cmdParts[0] == "up"):
			aim -= val
		else:
			eprint("unexpected cmd: {}".format(cmdParts[0]))
			continue
			
		eprint("x = {} and y = {}".format(x, y))
	
	print("p2 = x * y = {}".format(x * y))
	 
	f.close()


if __name__ == "__main__":
	main(sys.argv)
