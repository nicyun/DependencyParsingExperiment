#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <cassert>
#include <cstring>

#include "DependencyPaser.hpp"

using namespace std;

DependencyPaser::DependencyPaser()
{
	env = new Environment(1, 1, 1);
}

DependencyPaser::~DependencyPaser()
{
	delete env;
}

bool DependencyPaser::loadModel(const char * file)
{
	ifstream fin(file);
	int tot, numWord, numPos;
	string word, line;
	double value;
	fin >> tot;
	fin >> numWord;
	getline(fin, line);
	while(numWord--){
		getline(fin, line);
		istringstream sin(line);
		sin >> word;
		int wi = _getWordID(word);
		while(sin >> word >> value){
			int wj = _getWordID(word);
			env->addWordStat(wi, wj, value / tot);
		}
	}
	fin >> numPos;
	getline(fin, line);
	while(numPos--){
		getline(fin, line);
		istringstream sin(line);
		sin >> word;
		int wi = _getWordID(word);
		while(sin >> word >> value){
			int wj = _getWordID(word);
			env->addPosStat(wi, wj, value / tot);
		}
	}
	return true;
}

bool DependencyPaser::predictFile(const char * testFile, const char * outFile)
{
	ifstream fin(testFile);
	ofstream fout(outFile);
	string line;
	vector<vector<string> > sentence;
	while(getline(fin, line)){
		if(line == ""){
			vector<int> father;
			vector<string> words, postags;
			words.push_back("ROOT");
			postags.push_back("ORG");
			for(size_t i = 0; i < sentence.size(); i++){
				words.push_back(sentence[i][1]);
				postags.push_back(sentence[i][3]);
			}
			predict(words, postags, father);
			for(size_t i = 0; i < sentence.size(); i++){
				sentence[i][7] = "-";
				fout << sentence[i][0];
				for(size_t j = 1; j < sentence[i].size(); j++){
					if(j == 6) fout << "\t" << father[i+1];
					else fout << "\t" << sentence[i][j];
				}
				fout << endl;
			}
			fout << endl;
			sentence.clear();
		}
		else{
			vector<string> item;
			string tmp;
			istringstream sin(line);
			while(sin >> tmp){
				item.push_back(tmp);
			}
			sentence.push_back(item);
		}
	}
	return true;
}

double DependencyPaser::predict(const vector<string> & words,
		const vector<string> & postags, vector<int> & fathers)
{
	int n = words.size();
	vector<vector<double> > graph(n, vector<double>(n, 0));
	_buildGraph(words, postags, graph);
	return _eisner(graph, fathers);
}

bool DependencyPaser::_decode(
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

double DependencyPaser::_eisner(
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

int DependencyPaser::_getWordID(const string & word)
{
	if(strMap.find(word) == strMap.end()){
		int id = strMap.size();
		strMap[word] = id;
	}
	return strMap[word];
}

// build graph in a multiple-agent way
bool DependencyPaser::_buildGraph(const vector<string> & words,
		const vector<string> & postags, vector<vector<double> > & graph)
{	
	vector<WordAgent> wordAgents;
	Simulator simulator;
	env->resetAgents();
	for(size_t i = 0; i < words.size(); i++){
		vector<int> attr;
		attr.push_back(_getWordID(words[i])); 
		attr.push_back(_getWordID(postags[i]));
		int numClone = env->getNumClone();
		pair<int, int> pos = env->getRandomPosition();
		for(int j = 0; j < numClone; j++){
			wordAgents.push_back(WordAgent(i, attr, env, pos));
		}
	}
	// vector allocates new memeries sometimes.
	// so this {for} and the previous {for} could not be merged
	for(size_t i = 0; i < wordAgents.size(); i++){
		WordAgent * pWordAgent = &wordAgents[i];
		simulator.addPWordAgent(pWordAgent);
		env->addPWordAgent(pWordAgent);
	}
	simulator.run();
	graph.clear();
	int n = words.size();
	graph.resize(n, vector<double>(n, 0));
	for(size_t k = 0; k < wordAgents.size(); k++){
		int i = wordAgents[k].getID();
		map<int, double> & affinityTo = wordAgents[k].getAffinityTo();
		for(map<int, double>::iterator it = affinityTo.begin();
				it != affinityTo.end(); it++){
			int j = it->first;
			double value = it->second;
			assert(graph[i][j] == 0 || graph[i][j] == value);
			graph[i][j] = value;
		}
		map<int, double> & affinityFrom = wordAgents[k].getAffinityFrom();
		for(map<int, double>::iterator it = affinityFrom.begin();
				it != affinityFrom.end(); it++){
			int j = it->first;
			double value = it->second;
			assert(graph[j][i] == 0 || graph[j][i] == value);
			graph[j][i] = value;
		}
	}
	return true;
}
