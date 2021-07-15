#!/usr/bin/python
import sys

out1, out2 = sys.argv[1], sys.argv[2]
with open(out1, 'r') as f1:
	with open(out2, 'r') as f2:
		lines1 = f1.readlines()	
		lines2 = f2.readlines()
		if len(lines2) != len(lines1):
			print("TEST FAILED")
			exit()
		for ind in range(0, len(lines2)):
			if len(lines2[ind].strip()) > 0:
				line1 = [int(x) for x in lines1[ind].strip().split(" ")]
				line2 = [int(x) for x in lines2[ind].strip().split(" ")]
				if set(line1) != set(line2):
					print("TEST FAILED")
					exit()
print("Test passed")