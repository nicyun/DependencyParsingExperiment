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

bool WordAgent::_mutate()
{
	if(getStatus() == MUTATE)
	{
		tmpFeature = domFeature;
		bool flag = false;
		/*multi-point mutation*/
		int n = -1;
		double d = 0.0;
		for(size_t i = 0; i < mutatePosition.size(); i++)
		{
			n = rand()%2;
			if(n == 0)
			{
				/*increase*/
				d = domFeature[i] + DETA;
				if(d < 1.0)
				{
					tmpFeature[i] = d;
					flag = true;
				}
			}
			else
			{
				/*decrease*/
				d = domFeature[i] - DETA;
				if(d > 0.0)
				{
					tmpFeature[i] = d;
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
	/*calculating affinity between antigen and B cell*/
	double sum = 0.0;
	if(status == ACTIVE)
	{
		std::map<int, double>::iterator it;
		for(size_t i = 0; i < receptor.size(); i++)
		{
			it = domFeature.find(receptor[i]);
			if(it != domFeature.end())
			{
				sum += domFeature[receptor[i]];
			}
		}
	}
	return sum;
}

int WordAgent::getStatus()
{
	return status;
}

void WordAgent::setStatus(int s)
{
	status = s;
}

bool WordAgent::interact(std::vector<int> receptor)
{
	/*interaction between antigen and B cell*/
	if(_calAffinity(receptor) > AFFINITY)
	{
		setStatus(MUTATE);
	}

	/*interaction between B cells*/

	return true;
}

bool WordAgent::_clone()
{
	/*cloning by idiotype immune network and adaptive immune as equa:
	N = alpha*concentration + stimulus - suppression
	*/
	if(getStatus() == CLONE)
	{
		setStatus(MATURE);
		double alpha = 1.0 + agAffinity;

		int N = (int)(alpha*concentration) + stimulus - suppression;
		if(N > 0)
		{
			/*cloning*/
			for(int i = 0; i < N; i++)
			{
				/*new agent by cloning: status is MEMORY*/

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
			/*new agent by cloning: status is ACTIVE*/
			;
			/*add agent to environment*/
		}
	}
	else if(N < 0)
	{
		/*removing (suppression)*/
		int M = -N;
		for(int j = 0 ; j < M; j++)
		{
			/*removing agent*/
			;
		}

	}

	return true;
}

std::vector<int> WordAgent::getRecReceptor() const
{
	return recFeature;
}

bool WordAgent::select()
{
	if(getStatus() == MUTATE)
	{
		/*selection by competition among mutated agents:
		(1) agents generating negative feedbacks of depedency parsing are died;
		(2) agents with lower affinities are died when competing;
		*/
		if(_getFeedback())
		{
			/*gain neighbours*/
			std::vector<WordAgent *> nearAgents;
			if(env->getNearbyAgents(this, nearAgents))
			{
				double agAffinity = 0.0;
				for(size_t i = 0; i < nearAgents.size(); i++)
				{
					if(nearAgents[i]->getStatus() == MUTATE)
					{
						if(agAffinity < nearAgents[i]->getAgAffinity())
						{
							setStatus(DIE);
						}
						else
						{
							nearAgents[i]->setStatus(DIE);
						}
					}
				}

				if(getStatus() == MUTATE)
				{
					/*communicating with local environment*/
					domFeature = tmpFeature;
					_communicate();	
				}
			}
		}
		else
		{
			setStatus(DIE);
		}
	}

	return true;
}

bool WordAgent::_getFeedback()
{
	/*gain feedback of dependency parsing by local environment*/

	return true;
}

void WordAgent::_communicate()
{
	/*updating information of local environment*/
	env->updateLocalEnv(this);
}

double WordAgent::getAgAffinity()
{
	return agAffinity;
}

bool WordAgent::_die()
{
	if(getStatus() == DIE)
	{
		if(env->delPWordAgent(this))
		{
			return true;
		}
	}
	return false;
}

bool WordAgent::_getStimulus()
{
	/*get neighbour*/
	std::vector<WordAgent * > nearAgents;
	if(env->getNearbyAgents(this,nearAgents))
	{
		std::vector<int> rec;
		double affinity;
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			rec = nearAgents[i]->getRecReceptor();
			affinity = _calAffinity(rec);
			if(affinity > STIMULUS)
			{
				stimulus++;
			}
		}
	}
	
	return true;
}

bool WordAgent::_getSuppression()
{
	/*get neighbour*/
	std::vector<WordAgent *> nearAgents;
	if(env->getNearbyAgents(this,nearAgents))
	{
		std::map<int, double> rec;
		double affinity;
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			rec = nearAgents[i]->getDomReceptor();
			affinity = _calAffinity(rec);
			if(affinity > SUPPRESS)
			{
				suppression++;
			}
		}
	}

	return true;
}

double WordAgent::_calAffinity(std::map<int, double> receptor)
{
	double sum = 0.0;
	map<int, double>::iterator it;
	for(size_t i = 0; i < recFeature.size(); i++)
	{
		it = receptor.find(recFeature[i]);
		if(it != receptor.end())
		{
			sum += receptor[recFeature[i]];
		}		
	} 

	return sum;
}

std::map<int, double> WordAgent::getDomReceptor() const
{
	return domFeature;
}
