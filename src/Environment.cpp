#include <iostream>

#include <cstdlib>
#include <ctime>

#include "WordAgent.hpp"

using namespace std;

Environment::Environment(int r, int c, int cnt)
		: rows(r), cols(c), numClone(cnt)
{
	resetAgents();
}

int Environment::_calcSub(const pair<int, int> & pos) const
{
	return pos.first * cols + pos.second;
}

double Environment::calcAffinity(const vector<int> & attrs1, 
		const vector<int> & attrs2) const{
	double res = 0;
	map<pair<int, int>, double>::const_iterator it;
	it = wordStat.find(make_pair(attrs1[0], attrs2[0]));
	if(it != wordStat.end()) res += it->second * 0.50;
	it = posStat.find(make_pair(attrs1[1], attrs2[1]));
	if(it != posStat.end()) res += it->second * 0.50;
	return res;
}

bool Environment::addWordStat(int father, int son, double value)
{
	wordStat[make_pair(father, son)] = value;
	return true;
}

bool Environment::addPosStat(int father, int son, double value)
{
	posStat[make_pair(father, son)] = value;
	return true;
}

bool Environment::addPWordAgent(WordAgent * pWordAgent)
{
	pWordAgents[_calcSub(pWordAgent->getPosition())].insert(pWordAgent);
	return true;
}

bool Environment::delPWordAgent(WordAgent * pWordAgent)
{
	pWordAgents[_calcSub(pWordAgent->getPosition())].erase(pWordAgent);
	return true;
}

bool Environment::resetAgents()
{
	pWordAgents.clear();
	pWordAgents.resize(rows * cols, set<WordAgent *>());
	return true;
}

int Environment::agentCount(const pair<int, int> & pos)
{
	return pWordAgents[_calcSub(pos)].size();
}

bool Environment::getNearbyAgents(const WordAgent * pWordAgent,
		vector<WordAgent *> & neabyAgents) const
{
	neabyAgents.clear();
	const set<WordAgent *> & near = pWordAgents[_calcSub(pWordAgent->getPosition())];
	for(set<WordAgent *>::iterator it = near.begin();
			it != near.end(); it++){
		if(*it != pWordAgent){
			neabyAgents.push_back(*it);
		}
	}
	return true;
}

int Environment::getNumClone()
{
	return numClone;
}

pair<int, int> Environment::getRandomPosition()
{
	srand(time(NULL));
	int row = rand() % rows;
	srand(time(NULL));
	int col = rand() % cols;
	return make_pair(row, col);
}

bool Environment::xInRange(int x)
{
	return x >= 0 && x < rows;
}

bool Environment::yInRange(int y)
{
	return y >= 0 && y < cols;
}
