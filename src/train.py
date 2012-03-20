import sys

if __name__ == "__main__":
	model = {}
	head = [-1]
	word = ['ROOT']
#statistics
	for line in open(sys.argv[1]):
		line = line.strip().split()
		if(len(line) == 0):
			for i in xrange(1, len(word)):
				wj = word[i]
				wi = word[head[i]]
				if(wi not in model):
					model[wi] = {}
				if(wj not in model[wi]):
					model[wi][wj] = 0
				model[wi][wj] += 1
			head = [-1]
			word = ['ROOT']
			continue
		word.append(line[1])
		head.append(int(line[6]))
#write model
	out = open(sys.argv[2], 'w')
	for wi in model:
		line = wi
		for wj in model[wi]:
			line += "\t" + wj + ' ' + str(model[wi][wj])
		out.write(line + '\n')
	out.flush()

