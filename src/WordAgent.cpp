#include <vector>
#include <iostream>

#include <cassert>
#include <ctime>
#include <cstdlib>

#include "WordAgent.hpp"

using namespace std;

WordAgent::WordAgent(int id, Environment * environment, 
		const pair<int, int> & pos){
	ID = id;
	env = environment;
	position = pos;
	//orders.push(1);
}

int WordAgent::getID()
{
	return ID;
}

bool WordAgent::run()
{
	bool hasRun = false;
	return hasRun;
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

pair<int, int> WordAgent::getPosition() const
{
	return position;
}
