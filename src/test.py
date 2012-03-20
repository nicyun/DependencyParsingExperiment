import sys

def decode(f, s, t, d, c, g):
	res = {}
	if c == 0:
		for q in xrange(s, t + 1):
			if f[s][q][d][d] + f[q][t][d][1-d] == f[s][t][d][c]:
				if (q == t and d == c) or (q == s and 1 - d == c):
					continue
				res.update(decode(f, s, q, d, d, g))
				res.update(decode(f, q, t, d, 1 - d, g))
				return res
	else:
		i = t
		j = s
		if d:
			i = s
			j = t
		for q in xrange(s, t):
			if f[s][t][d][c] == f[s][q][1][0] + f[q+1][t][0][0] + g[i][j]:
				res[j] = i
				res.update(decode(f, s, q, 1, 0, g))
				res.update(decode(f, q + 1, t, 0, 0, g))
				return res
	return res

def Eisner(g):
#Eisner's algorithm for MST
	n = len(g)
	f = [[[[0 for a in xrange(2)] for b in xrange(2)] for c in xrange(n)] for d in xrange(n)]
	for k in xrange(1, n):
		for s in xrange(n - k):
			t = s + k
			for q in xrange(s, t):
				f[s][t][0][1] = max(f[s][q][1][0] + f[q+1][t][0][0] + g[t][s], f[s][t][0][1])
				f[s][t][1][1] = max(f[s][q][1][0] + f[q+1][t][0][0] + g[s][t], f[s][t][1][1])
			for q in xrange(s, t + 1):
				f[s][t][0][0] = max(f[s][q][0][0] + f[q][t][0][1], f[s][t][0][0])
				f[s][t][1][0] = max(f[s][q][1][1] + f[q][t][1][0], f[s][t][1][0])
	return decode(f, 0, n - 1, 1, 0, g)
				
def predict(w_model, t_model, words, tags):
#pre-defined interpolation value ??
	mu = 0.99
#build graph
	tmp = [0] * len(words)
	g = [list(tmp) for i in xrange(len(words))]
	for i in xrange(len(words)):
		if(words[i] in w_model):
			for j in xrange(len(words)):
				if(i == j):
					continue
				if(words[j] in w_model[words[i]]):
					g[i][j] += w_model[words[i]][words[j]] * mu
		if(tags[i] in t_model):
			for j in xrange(len(tags)):
				if(i == j):
					continue
				if(tags[j] in t_model[tags[i]]):
					g[i][j] += t_model[tags[i]][tags[j]] * (1 - mu)
#test graph??
#estimate MST
	return Eisner(g)

if __name__ == "__main__":
#load model
	w_model = {}
	t_model = {}
	fin = open(sys.argv[1])
	tot = int(fin.readline().strip())
	cnt = int(fin.readline().strip())
	for k in xrange(cnt):
		line = fin.readline().strip().split('\t')
		w_model[line[0]] = {}
		for i in xrange(1, len(line)):
			pair = line[i].strip().split()
			w_model[line[0]][pair[0]] = float(pair[1]) / tot
	cnt = int(fin.readline().strip())
	for k in xrange(cnt):
		line = fin.readline().strip().split('\t')
		t_model[line[0]] = {}
		for i in xrange(1, len(line)):
			pair = line[i].strip().split()
			t_model[line[0]][pair[0]] = float(pair[1]) / tot
#read sentence
	word = ["ROOT"]
	tag = ["ROOT"]
	lines = []
	res = []
	for line in open(sys.argv[2]):
		line = line.strip().split()
		if(len(line) == 0):
			head = predict(w_model, t_model, word, tag)
			for line in lines:
				line[6] = str(head[int(line[0])])
				line[7] = '-'
				res.append('\t'.join(line) + '\n')
			res.append('\n')
			word = ["ROOT"]
			tag = ["ROOT"]
			lines = []
			continue
		word.append(line[1])
		tag.append(line[3])
		lines.append(line)
	open(sys.argv[3], 'w').writelines(res)
