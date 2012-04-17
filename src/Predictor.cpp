#include <cassert>
#include <cstring>

#include "Predictor.hpp"

using namespace std;

Predictor::Predictor(Model * pm) : pModel(pm)
{
}

bool Predictor::_decode(
		const double f[maxLen][maxLen][2][2], 
		int s, int t, int d, int c,
		const vector<vector<double> > & g,
		std::vector<int> & father)
{
	if(!c){
		for(int q = s; q <= t; q++){
			if(f[s][q][d][d] + f[q][t][d][1-d] == f[s][t][d][c]){
				if((q == t && d == c) || (q == s && 1 - d == c)){
					continue;
				}
				_decode(f, s, q, d, d, g, father);
				_decode(f, q, t, d, 1 - d, g, father);
				break;
			}
		}
	}
	else{
		int i = t, j = s;
		if(d){
			i = s, j = t;
		}
		for(int q = s; q < t; q++){
			if(f[s][t][d][c] == f[s][q][1][0] + f[q+1][t][0][0] + g[i][j])
			{
				father[j] = i;
				_decode(f, s, q, 1, 0, g, father);
				_decode(f, q + 1, t, 0, 0, g, father);
				break;
			}
		}
	}
	return true;
}

double Predictor::_eisner(
		const vector<vector<double> > & graph,
		vector<int> & father)
{
	int n = graph.size();
	assert(n < maxLen);
	double f[maxLen][maxLen][2][2];
	memset(f, 0, sizeof(f));
	for(int k = 1; k < n; k++){
		for(int s = 0; s < n - k; s++){
			int t = s + k;
			for(int q = s; q < t; q++){
				f[s][t][0][1] = max(f[s][t][0][1], f[s][q][1][0] + f[q+1][t][0][0] + graph[t][s]);
				f[s][t][1][1] = max(f[s][t][1][1], f[s][q][1][0] + f[q+1][t][0][0] + graph[s][t]);
			}
			for(int q = s; q <= t; q++){
				f[s][t][0][0] = max(f[s][t][0][0], f[s][q][0][0] + f[q][t][0][1]);
				f[s][t][1][0] = max(f[s][t][1][0], f[s][q][1][1] + f[q][t][1][0]);
			}
		}
	}
	father.resize(n, -1);
	_decode(f, 0, n - 1, 1, 0, graph, father);
	return f[0][n-1][1][0];
}

bool Predictor::_buildGraph(const Sentence & sen, 
		vector<vector<double> > & graph)
{	
	graph.clear();
	int n = sen.size();
	graph.resize(n, vector<double>(n, 0));
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(j == i) continue;
			graph[i][j] = pModel->wordPairWeight(sen, i, j);
		}
	}
	return true;
}

double Predictor::predict(const Sentence & sen, vector<int> & fa)
{
	vector<vector<double> > graph;
	_buildGraph(sen, graph);
	return _eisner(graph, fa);
}
