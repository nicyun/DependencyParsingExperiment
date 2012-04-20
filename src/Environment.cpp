#include <iostream>

#include <cstdlib>
#include <ctime>

#include "WordAgent.hpp"
#include "Information.hpp"
#include "Parameter.hpp"

using namespace std;

Environment::Environment(int r, int c)
		: rows(r), cols(c)
{
	resetAgents();
}

int Environment::_calcSub(const pair<int, int> & pos) const
{
	return pos.first * cols + pos.second;
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

bool Environment::update(WordAgent * pWordAgent)
{
	if(!infor->getFeedback(pWordAgent))
    	{
		pWordAgent->setStatus(DIE);
		return false;
	}

	return true;
}

std::map<int, double> Environment::getInfor(WordAgent * pWordAgent)
{
	return infor->getInfor(pWordAgent);
}
