#include <vector>
#include <iostream>

#include <cassert>

#include "WordAgent.hpp"

using namespace std;

bool WordAgent::init(int id, const vector<int> & attributes,
		const Environment * environment){
	ID = id;
	attrs = attributes;
	env = environment;
	orders.push(1);
	return true;
}

double WordAgent::affinityToFather(WordAgent * pFather)
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

bool WordAgent::run()
{
	bool hasRun = false;
	while(orders.size()){
		int now = orders.front();
		orders.pop();
		switch (now){
			case 1:
				_calcAffinities();
				break;
			default:
				assert(0);
		}
		hasRun = true;
	}
	return hasRun;
}

bool WordAgent::_calcAffinities()
{
	affinityTo.clear();
	affinityFrom.clear();
	vector<WordAgent *> nearAgents;
	env->getNearbyAgents(nearAgents);
	for(size_t i = 0; i < nearAgents.size(); i++){
		int p = nearAgents[i]->getID();
		if(p == ID) continue;
		double toFather = affinityToFather(nearAgents[i]);
		double toSon = affinityToSon(nearAgents[i]);
		affinityTo[p] = toSon;
		affinityFrom[p] = toFather;
	}
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
