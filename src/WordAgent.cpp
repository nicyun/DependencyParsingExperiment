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

	stimulus = 0.0;
	suppression = 0.0;
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
	if(getStatus() == MATCH)
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

		/*calculating affinity after mutation*/
		double mutatedAffinity = _calMutatedAffinity(agFeature);

		if(flag && _calFeedback() && (mutatedAffinity > agAffinity))
		{
			setStatus(MUTATE);
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

double WordAgent::_calMutatedAffinity(std::vector<int> receptor)
{
	/*calculating affinity after mutation*/
	double sum = 0.0;
	std::map<int, double>::iterator it;
	for(size_t i = 0; i < receptor.size(); i++)
	{
		it = tmpFeature.find(receptor[i]);
		if(it != tmpFeature.end())
		{
			sum += tmpFeature[receptor[i]];
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

bool WordAgent::_interact()
{
	/*interaction between word-agents:
	(1) Antigens and B cells;
	(2) B cells
	*/

	if(getStatus() != ACTIVE)
	{
		return false;
	}

	/*gain near agents*/
	std::vector<WordAgent *> nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{
		/*selecting objected interacted randomly*/
		for(size_t i = 0; i < nearAgents.size(); i++ )
		{
			if(nearAgents[i]->getCategory() == ANTIGEN)
			{
				/*interacting between Antigens and B cells*/
				if(nearAgents[i]->getStatus() == ACTIVE)
				{
					this->agFeature = nearAgents[i]->getRecReceptor();
					if(_calAffinity(this->agFeature) > AFFINITY)	
					{
						nearAgents[i]->setStatus(DIE);
						setStatus(MATCH);
					}
				}
			}

			if(getStatus() != ACTIVE)
			{
				break;
			}
		}
	}

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
		if(!_getRegulation())
		{
			std::cerr<<"Gain regulation failed!"<<std::endl;
		}
		double alpha = 1.0 + agAffinity;

		int N = (int)(alpha + stimulus - suppression) * concentration;
		if(N > 0)
		{
			/*cloning*/
			for(int i = 0; i < N; i++)
			{
				/*new agent by cloning: status is MEMORY*/
				WordAgent * wa = new WordAgent(ID,env,position);
				/*add agent to environment*/
				env->addPWordAgent(wa);
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
	if(_getRegulation())
	{
		std::cerr<<"Gain regulation failed!"<<std::endl;
	}
	int N = concentration * (int)(1.0 + stimulus - suppression);

	if(N > 0)
	{
		/*cloning (stimulus)*/
		for(int i = 0; i < N; i++)
		{
			/*new agent by cloning: status is ACTIVE*/
			WordAgent * wa = new WordAgent(ID,env,position);
			;
			/*add agent to environment*/
			env->addPWordAgent(wa);
		}
	}
	else if(N < 0)
	{
		std::vector<WordAgent * > nearAgents;
		int M = -N;
		if(env->getNearbyAgents(this,nearAgents))
		{
			for(size_t i = 0; i < nearAgents.size(); i++)
			{
				if(nearAgents[i]->getID() == ID)
				{
					env->delPWordAgent(this);
					M--;
					if(M == 0)
					{
						break;
					}
				}
			}
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
		/*gain neighbours*/
		std::vector<WordAgent *> nearAgents;
		if(env->getNearbyAgents(this, nearAgents))
		{
			for(size_t i = 0; i < nearAgents.size(); i++)
			{
				if(nearAgents[i]->getStatus() == MUTATE)
				{
					/*comparing feedbacks*/
					if(_cmpFeedback(feedback,nearAgents[i]->getFeedback()))
					{
						setStatus(DIE);
					}
					else
					{
						nearAgents[i]->setStatus(DIE);
					}
				}
			}
               		/*communicating with local environment to insure global optimization*/
                	_communicate();
		}
	}

	return true;
}

bool WordAgent::_calFeedback()
{
	/*calculating feedback based on mutated receptors*/
	if(getStatus() != MUTATE)
	{
		return false;
	}

	feedback = env->gainFeedback(this);

	return true;
}
std::pair<int, double> WordAgent::getFeedback() const
{
	return this->feedback;
}

void WordAgent::_communicate()
{
	/*updating information of local environment*/
	if(env->update(this))
	{
		setStatus(MATURE);
	}
	else
	{
		setStatus(DIE);
	}
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
	stimulus = 0.0;
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
				stimulus += affinity;
			}
		}
	}

	return true;
}

bool WordAgent::_getSuppression()
{
	suppression = 0.0;
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
				suppression += affinity;
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

bool WordAgent::_updateSelf()
{
	/*updating information of self receptor including:
	domFeature
	*/
	std::map<int, double> rec = env->getInfor(this);
	std::map<int, double>::iterator it;
	std::map<int, double>::iterator tmp;
	for(it = rec.begin(); it != rec.end(); it++)
	{
		tmp = domFeature.find(it->first);
		if(tmp != domFeature.end())
		{
			if(domFeature[it->first] != it->second)
			{
				domFeature[it->first] = it->second;
			}
		}
	}

	return true;
}

bool WordAgent::_cmpFeedback(std::pair<int, double> sp, std::pair<int, double> dp)
{
	if(sp.first < dp. second)
	{
		return false;
	}

	return true;

}

int WordAgent::getCategory()
{
	return this->category;
}

void WordAgent::gainSuppression(double suppress)
{
	this->suppression += suppress;
}

bool WordAgent::_getRegulation()
{
	/*get near agents*/
	std::vector<WordAgent *> nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{
		double affinity = 0.0;
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			if(nearAgents[i]->getStatus() != DIE)
			{
				affinity = _calAffinity(nearAgents[i]->getRecReceptor());
				stimulus += affinity;
				nearAgents[i]->gainSuppression(affinity);
			}
		}
	}
	return true;
}
