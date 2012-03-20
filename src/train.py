import sys

if __name__ == "__main__":
	w_model = {}
	t_model = {}
	tot = 0
	head = [-1]
	word = ['ROOT']
	tag = ['ROOT']
#statistics
	for line in open(sys.argv[1]):
		line = line.strip().split()
		if(len(line) == 0):
			for i in xrange(1, len(word)):
				wj = word[i]
				tj = tag[i]
				ti = tag[head[i]]
				wi = word[head[i]]
				if(wi not in w_model):
					w_model[wi] = {}
				if(ti not in t_model):
					t_model[ti] = {}
				if(wj not in w_model[wi]):
					w_model[wi][wj] = 0
				if(tj not in t_model[ti]):
					t_model[ti][tj] = 0
				w_model[wi][wj] += 1
				t_model[ti][tj] += 1
				tot += 1
			head = [-1]
			word = ['ROOT']
			tag = ['ROOT']
			continue
		word.append(line[1])
		tag.append(line[3])
		head.append(int(line[6]))
#write w_model
	out = open(sys.argv[2], 'w')
	out.write(str(tot) + '\n')
	out.write(str(len(w_model)) + '\n')
	for wi in w_model:
		line = wi
		for wj in w_model[wi]:
			line += "\t" + wj + ' ' + str(w_model[wi][wj])
		out.write(line + '\n')
	out.write(str(len(t_model)) + '\n')
	for ti in t_model:
		line = ti
		for tj in t_model[ti]:
			line += "\t" + tj + ' ' + str(t_model[ti][tj])
		out.write(line + '\n')
	out.flush()

