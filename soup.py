#!/usr/bin/env python3
import os
import sys
import json
import string
from subprocess import check_output
from collections import defaultdict
from copy import deepcopy

numbers = {
	1: "one",
	2: "two",
	3: "three",
	4: "four",
	5: "five",
	6: "six",
	7: "seven",
	8: "eight",
	9: "nine",
	10: "ten",
	11: "eleven",
	12: "twelve",
	13: "thirteen",
	14: "fourteen",
	15: "fifteen",
	16: "sixteen",
	17: "seventeen",
	18: "eighteen",
	19: "nineteen",
	20: "twenty",
	30: "thirty",
	40: "forty"
}

def lookup(num):
	if num > 40:
		return numbers[40] + "-" + lookup (num - 40)
	elif num > 30:
		return numbers[30] + "-" + lookup (num - 30)
	elif num > 20:
		return numbers[20] + "-" + lookup (num - 20)
	if num in numbers:
		return numbers[num]
	return num


def dist_to_list(d):
	"""turns a distribution map into a list of ints"""
	l=[]
	for letter in string.ascii_lowercase:
		if letter in d:
			l.append(d[letter])
		else:
			l.append(0)
	return l


def str_to_dist(s):
	"""turns a string into a distribution map"""
	d=defaultdict(int)
	for c in s:
		if c in string.ascii_lowercase:
			d[c]=d[c]+1
	return d


def list_to_dist(l):
	"""turns a list of ints into a distribution map"""
	d = {}
	for letter in string.ascii_lowercase:
		d[letter] = l.pop(0)
	return d


if __name__ == '__main__':
	words = sys.argv[1:]
	# Seed the final result
	result = " ".join(words) + " "
	words.append("and")

	seedstring = ''.join(words).lower()
	#print ("seedstring: " + seedstring)
	seeddist = str_to_dist(seedstring)

	charlist = dist_to_list(seeddist)
	outjson = { "sentenceSeed": charlist }

	# check if these intermediaries exist:
	if not os.path.exists("unoptimized.json"):
		os.system("sentient -c self-enumerating-pangram.snt > unoptimized.json")
	if not os.path.exists("optimized.json"):
		os.system("sentient -o unoptimized.json > optimized.json")
	solvedjson = check_output(["sentient", "--run","optimized.json","--assign",json.dumps(outjson),"--machine","lingeling"])
	solvedjson = solvedjson.decode("utf-8").rstrip()
	solved = json.loads(solvedjson)
	"""
	with open("out.json", "w+") as outfile:
		outfile.write(json.dumps(solved))
	outfile = open("out.json")
	out = outfile.read()
	solved = json.loads(out)
	"""

	if "letterCounts" not in solved:
		print("no solution", file=sys.stderr)
		sys.exit(-1)

	lettercounts = solved["letterCounts"]
	resdist = list_to_dist(deepcopy(lettercounts))

	#split into list and final element
	usedletterdict = { k: resdist[k] for k in resdist if resdist[k] > 0 }
	usedletters = list(usedletterdict.keys())
	finalletter = usedletters[-1:]
	allbutlast = usedletters[:-1]

	for letter in allbutlast:
		count = resdist[letter]
		if count > 1:
			eng = lookup(count)
			result = result + "%s %s's, " % (eng, letter)
		elif count > 0:
			eng = lookup(count)
			result = result + "%s %s, " % (eng, letter)

	# end the sentence
	result = result + "and "
	letter = finalletter[0]
	count = resdist[letter]
	if count > 1:
		eng = lookup(count)
		result = result + "%s %s's." % (eng, letter)
	elif count > 0:
		eng = lookup(count)
		result = result + "%s %s." % (eng, letter)

	# turn a string into a distribution
	resdist=str_to_dist(result.lower())
	# and a distribution into a list of ints
	reslist=dist_to_list(resdist)
	if reslist != lettercounts:
		print("mismatched letter counts!", file=sys.stderr)
		print('solver:  ' + json.dumps(lettercounts), file=sys.stderr)
		print('results: ' + json.dumps(reslist), file=sys.stderr)
		sys.exit(-1)
	print(result)
