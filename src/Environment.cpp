#include <iostream>

#include "Environment.hpp"

using namespace std;

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

bool Environment::addPWordAgent(class WordAgent * pWordAgent)
{
	pWordAgents.push_back(pWordAgent);
	return true;
}

bool Environment::clearAgents()
{
	pWordAgents.clear();
	return true;
}

bool Environment::getNearbyAgents(vector<class WordAgent *> & neabyAgents) const
{
	neabyAgents = pWordAgents;
	return true;
}
