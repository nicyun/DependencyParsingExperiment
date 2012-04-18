#include <vector>
#include <iostream>

#include <cassert>
#include <ctime>
#include <cstdlib>
#include <math.h>

#include "Parameter.hpp"
#include "WordAgent.hpp"

using namespace std;

WordAgent::WordAgent(int id, Environment * environment, 
		const pair<int, int> & pos){
	ID = id;
	env = environment;
	position = pos;
	status = ACTIVE;
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

bool WordAgent::_mutate(std::vector<int> mutatePosi, std::map<int,double> & f)
{
	if(_getStatus() == MUTATE)
	{
		bool flag = false;
		/*multi-point mutation*/
		int n = -1;
		double d = 0.0;
		for(size_t i = 0; i < mutatePosi.size(); i++)
		{
			n = rand()%2;
			if(n == 0)
			{
				/*increase*/
				d = domFeature[i] + DETA;
				if(d < 1.0)
				{
					f[i] = d;
					flag = true;
				}
			}
			else
			{
				/*decrease*/
				d = domFeature[i] - DETA;
				if(d > 0.0)
				{
					f[i] = d;
					flag = true;
				}
			}
			d = 0.0;
		}

		if(flag)
		{
			return true;
		}
	}

	return false;
}

double WordAgent::_calAffinity(std::vector<int> receptor)
{
	double sum = 0.0;
	std::map<int, double>::iterator it;
	for(size_t i = 0; i < receptor.size(); i++)
	{
		it = domFeature.find(receptor[i]);
		if(it != domFeature.end())
		{
			sum += domFeature[receptor[i]];
		}
	}

	return sum;
}

int WordAgent::_getStatus()
{
	return status;
}

void WordAgent::_setStatus(int s)
{
	status = s;
}

bool WordAgent::interact(std::vector<int> receptor)
{
	if(_calAffinity(receptor) > AFFINITY)
	{
		_setStatus(MUTATE);
	}

	return true;
}

bool WordAgent::_clone()
{
	/*cloning by idiotype immune network and adaptive immune as equa:
	N = alpha*concentration + stimulus - suppression
	*/
	if(_getStatus() == CLONE)
	{
		double alpha = 1.0 + domAffinity;

		int N = (int)(alpha*concentration) + stimulus - suppression;
		if(N > 0)
		{
			_setStatus(MATURE);
			/*cloning*/
			for(int i = 0; i < N; i++)
			{
				/*new agent by cloning*/

				/*add agent to environment*/

			}

			return true;

		}
	}

	return false;
}

bool WordAgent::_regulate()
{
	/*regulating by idiotype immune network as equa:
	N = concentration + stimulus - suppression
	*/
	int N = concentration + stimulus - suppression;

	if(N > 0)
	{
		/*cloning (stimulus)*/
		for(int i = 0; i < N; i++)
		{
			/*new agent by cloning*/

			/*add agent to environment*/
		}
	}
	else if(N < 0)
	{
		/*removing (suppression)*/
		int M = -N;
		for(int i = 0 ; i < M; i++)
		{
			/*removing agent*/
		}

	}

	return true;
}
