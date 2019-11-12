#!/usr/bin/env python3
import os
import sys
import json
import string

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
		return numbers[40] + " " + lookup (num - 40)
	elif num > 30:
		return numbers[30] + " " + lookup (num - 30)
	elif num > 20:
		return numbers[20] + " " + lookup (num - 20)
	if num in numbers:
		return numbers[num]
	return num

def reanalyze(s):
	resdist={}
	for c in string.ascii_lowercase:
		resdist[c] = 0
	for c in s:
		if c in string.ascii_lowercase:
			resdist[c]=resdist[c]+1
	reslist=[]
	for c in string.ascii_lowercase:
		reslist.append(resdist[c])
	print(json.dumps(reslist))

if __name__ == '__main__':
	chars = {}
	for letter in string.ascii_lowercase:
		chars[letter] = 0

	words = sys.argv[1:]
	result = " ".join(sys.argv[1:]) + " "

	words.append("and")

	inString = ''.join(words)

	for letter in inString:
		if letter in string.ascii_lowercase:
			chars[letter] = chars[letter] + 1

	charlist = []
	for letter in string.ascii_lowercase:
		charlist.append(chars[letter])

	outjson = { "sentenceSeed": charlist }

	#os.system("sentient -c self-enumerating-pangram.snt > unoptimized.json")
	#os.system("sentient -o unoptimized.json > optimized.json")
	#os.system("sentient --run optimized.json --assign '%s' --machine lingeling > out.json" % json.dumps(outjson))
	with open ("out.json") as solfile:
		sol = solfile.read()
		soldist = json.loads(sol)
		if "letterCounts" not in soldist:
			print("no solution", file=sys.stderr)
			sys.exit(-1)
		soldist = soldist["letterCounts"]
		resdist = {}
		for letter in string.ascii_lowercase:
			count = soldist.pop(0)
			resdist[letter] = count

		#split into list and final element
		usedletters = []
		for letter in string.ascii_lowercase:
			if resdist[letter] > 0:
				usedletters.append(letter)
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
		print(result)
		reanalyze(result,soldist)

# [3, 1, 3, 2, 38, 6, 3, 11, 9, 1, 1, 3, 1, 18, 13, 2, 1, 12, 25, 21, 4, 4, 6, 3, 4, 1]
# [3, 1, 3, 2, 38, 6, 3, 11, 9, 1, 1, 3, 1, 18, 13, 2, 1, 12, 25, 21, 4, 4, 6, 3, 4, 1]