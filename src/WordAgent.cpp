#include <vector>
#include <iostream>

#include <cassert>
#include <ctime>
#include <cstdlib>

#include "WordAgent.hpp"

using namespace std;

WordAgent::WordAgent(int id, const vector<int> & attributes,
		Environment * environment, const pair<int, int> & pos){
	ID = id;
	attrs = attributes;
	env = environment;
	position = pos;
	//orders.push(1);
}

double WordAgent::affinityFromFather(WordAgent * pFather)
{
	vector<int> attrs2;
	pFather->getAttributes(attrs2);
	return env->calcAffinity(attrs2, attrs);
}

double WordAgent::affinityToSon(WordAgent * pSon)
{
	vector<int> attrs2;
	pSon->getAttributes(attrs2);
	return env->calcAffinity(attrs, attrs2);
}

bool WordAgent::getAttributes(vector<int> & attrs2)
{
	attrs2 = attrs;
	return true;
}

int WordAgent::getID()
{
	return ID;
}

bool WordAgent::runByOrder()
{
	bool hasRun = false;
	while(orders.size()){
		int now = orders.front();
		orders.pop();
		switch (now){
			case 1:
				_calcAffinities();
				break;
			case 2:
				_doMove();
				break;
			default:
				assert(0);
		}
		hasRun = true;
	}
	return hasRun;
}

bool WordAgent::run()
{
	bool hasRun = false;
	_calcAffinities();
	while(_doMove()){
		hasRun = true;
		_calcAffinities();
	}
	return hasRun;
}

bool WordAgent::_calcAffinities()
{
	affinityTo.clear();
	affinityFrom.clear();
	vector<WordAgent *> nearAgents;
	env->getNearbyAgents(this, nearAgents);
	for(size_t i = 0; i < nearAgents.size(); i++){
		int p = nearAgents[i]->getID();
		if(p == ID) continue;
		double fromFather = affinityFromFather(nearAgents[i]);
		double toSon = affinityToSon(nearAgents[i]);
		affinityTo[p] = toSon;
		affinityFrom[p] = fromFather;
	}
	return true;
}

bool WordAgent::_doMove()
{
	static const int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
	static const int dy[] = {1, 1, 0, -1, -1, -1, 0, 1};
	int min = env->agentCount(position) - 1;
	vector<pair<int, int> > pos;
	for(int k = 0; k < 8; k++){
		int x = position.first + dx[k];
		int y = position.second + dy[k];
		pair<int, int> newPos = make_pair(x, y);
		if(env->xInRange(x) && env->yInRange(y)){
			if(env->agentCount(newPos) < min){
				min = env->agentCount(newPos);
				pos.clear();
			}
			if(env->agentCount(newPos) == min){
				pos.push_back(newPos);
			}
		}
	}
	if(min == env->agentCount(position) - 1){
		return false;
	}
	srand(time(NULL));
	int p = rand() % pos.size();
	pair<int, int> oldPos = position;
	env->delPWordAgent(this);
	position = pos[p];
	env->addPWordAgent(this);
	return true;
}

map<int, double> & WordAgent::getAffinityTo()
{
	return affinityTo;
}

map<int, double> & WordAgent::getAffinityFrom()
{
	return affinityFrom;
}

pair<int, int> WordAgent::getPosition() const
{
	return position;
}
