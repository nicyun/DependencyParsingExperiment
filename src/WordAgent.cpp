
#include <vector>
#include <iostream>

#include <cassert>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <iomanip>

#include "Parameter.hpp"
#include "WordAgent.hpp"

using namespace std;

WordAgent::WordAgent(int id, Environment * environment,
		const pair<int, int> & pos, int cat, int con){
	ID = id;
	env = environment;
	position = pos;

	category = cat;
	concentration = con;
	status = ACTIVE;

	stimulus = 0.0;
	suppression = 0.0;

	agAffinity = 0.0;

	_mapStatusToBehavior();

	/*init domFeature and recFeature*/
		
}

int WordAgent::getID()
{
	return ID;
}

bool WordAgent::run()
{
	bool hasRun = false;
	
	while(orders.size())
	{
		int now = orders.front();
		orders.pop();
		switch(now)
		{
			case MOVING:
				_doMove();
				break;
			case INTERACTING:
				_interact();
				break;
			case MUTATING:
				_mutate();
				break;
			case SELECTING:
				_select();
				break;
			case CLONING:
				_clone();
				break;
			case REGULATING:
				_regulate();
				break;
			case DYING:
				_die();
				break;
			default:
				assert(0);
		}
		hasRun = true;
	}
	return hasRun;
}

bool WordAgent::_doMove()
{
	_updateSelf();
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
	_mapStatusToBehavior();
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
		double mutatedProb = 0.0;
		srand(time(NULL));
		for(size_t i = 0; i < agFeature.size(); i++)
		{
		    /*producting mutated probability*/
		    int r = rand()%100;

		    mutatedProb = (double)r/100.0;
		    mutatedProb = 0.0;
		    if(mutatedProb < MUTATEPRO)
		    {

               		 n = rand()%2;
               		 if(n == 0)
               		 {
                   		 /*increase*/
                   		 d = domFeature[agFeature[i]] + DETA;
                   		 if(d < 1.0)
                   		 {
                       			 tmpFeature[agFeature[i]] = d;
					 mutatePosition.push_back(agFeature[i]);
                       			 flag = true;
                   		 }
               		 }
               		 else
               		 {
                   		 /*decrease*/
                   		 d = domFeature[agFeature[i]] - DETA;
                   		 if(d > 0.0)
                   		 {
                       			 tmpFeature[agFeature[i]] = d;
					 mutatePosition.push_back(agFeature[i]);
                       			 flag = true;
                   		 }
               		 }
               		 d = 0.0;
		    }
		}

		/*calculating difference of affinity after mutation*/
		mutatedAffinity = _calMutatedAffinity(agFeature);
		int diff = (mutatedAffinity - agAffinity) * PRECISION;

		/*if(_calFeedback() && (diff > 0))*/
		if(diff > 0)
		{
			setStatus(MUTATE);	
		}
	}
	_mapStatusToBehavior();

	return true;
}

double WordAgent::_calAffinity(std::vector<int> receptor)
{
	/*calculating affinity*/
	double sum = 0.0;

	size_t matchSize = 0;
	if(status == ACTIVE)
	{
		std::map<int, double>::iterator it;
		for(size_t i = 0; i < receptor.size(); i++)
		{
			it = domFeature.find(receptor[i]);
			if(it != domFeature.end())
			{

				matchSize++;
				sum += domFeature[receptor[i]];
			}
		}
	}
	if(matchSize == receptor.size())
	{
	    	setStatus(MATCH);
    	}
    
	return sum;
}

double WordAgent::_calStimulusByBcell(std::vector<int> receptor)
{
	double sum = 0.0;
	if(status != DIE)
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

	if(status != ACTIVE)
	{
		return false;
	}
	_updateSelf();

	/*gain near agents*/
	mutatePosition.clear();
	std::vector<WordAgent *> nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{
		/*selecting objected interacted randomly*/
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			if(nearAgents[i]->getCategory() == ANTIGEN)
			{
				/*interacting between Antigens and B cells*/
				if(nearAgents[i]->getStatus() == ACTIVE)
				{
					this->agFeature = nearAgents[i]->getRecReceptor();
					agAffinity = _calAffinity(this->agFeature);

					if(status == MATCH)
					{
						nearAgents[i]->setStatus(DIE);
						break;
					}
				}
			}
		}
	}
	_mapStatusToBehavior();

	return true;
}

bool WordAgent::_clone()
{
	/*cloning by idiotype immune network and adaptive immune as equa:
	N = alpha*concentration + stimulus - suppression
	*/
	if(getStatus() == MATURE)
	{
		setStatus(CLONE);
		if(!_getRegulation())
		{
			std::cerr<<"Gain regulation failed!"<<std::endl;
		}
		double alpha = 1.0 + agAffinity;
		int con = _calConcentration();
		int N = (int)(alpha + stimulus - suppression) * con;

		if(N > 0)
		{
			/*cloning*/
			for(int i = 0; i < N; i++)
			{
				/*new agent by cloning: status is MEMORY*/
				WordAgent * wa = new WordAgent(ID,env,position,BCELL,1);
				/*add agent to environment*/
				env->addPWordAgent(wa);
			}

			return true;
		}
	}
	_mapStatusToBehavior();

	return false;
}

bool WordAgent::_regulate()
{

	/*regulating by idiotype immune network as equa:
	N = concentration + stimulus - suppression
	*/
	_updateSelf();
	if(!_getRegulation())
	{
		std::cerr<<"Gain regulation failed!"<<std::endl;
	}

	int con = _calConcentration();
	int N = con * (int)(1.0 + stimulus - suppression);


	if(N > 0)
	{
		/*cloning (stimulus)*/
		for(int i = 0; i < N; i++)
		{
			/*new agent by cloning: status is ACTIVE*/
			WordAgent * wa = new WordAgent(ID,env,position,BCELL,1);

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
	_mapStatusToBehavior();

	return true;
}

std::vector<int> WordAgent::getRecReceptor() const
{
	return recFeature;
}

bool WordAgent::_select()
{
	if(status == MUTATE)
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
		/*			if(_cmpFeedback(feedback,nearAgents[i]->getFeedback()))*/
					if(mutatedAffinity < nearAgents[i]->getMutatedAffinity())
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
	_mapStatusToBehavior();

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
//	if(env->update(this))
	if(status == MUTATE)
	{
		setStatus(MATURE);
		domFeature = tmpFeature;
	}
/*	else
	{
		setStatus(DIE);
	}
	*/
}

double WordAgent::getAgAffinity()
{
	return agAffinity;
}

bool WordAgent::_die()
{
	if(status == DIE)
	{
		if(env->delPWordAgent(this))
		{
			return true;
		}
	}
	return false;
}

double WordAgent::_calSuppressByBcell(std::map<int, double> receptor)
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
	if(sp.first < dp.first)
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
	stimulus = 0.0;
	suppression = 0.0;
	/*get near agents*/
	std::vector<WordAgent *> nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{

		double affinity = 0.0;
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			if((nearAgents[i]->getStatus() != DIE) && (nearAgents[i]->getCategory() != ANTIGEN))
			{

				affinity = _calStimulusByBcell(nearAgents[i]->getRecReceptor());
				stimulus += affinity;
				
				affinity = _calSuppressByBcell(nearAgents[i]->getDomReceptor());
				suppression += affinity;
			}
		}
	}
	return true;
}

double WordAgent::getMutatedAffinity()
{
	return mutatedAffinity;
}

int WordAgent::getConcentration()
{
	return _calConcentration();
}

int WordAgent::_calConcentration()
{
	concentration = 1;
	vector<WordAgent * > nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			if(nearAgents[i]->getStatus() != DIE)
			{
				if(ID == nearAgents[i]->getID())
				{
					concentration++;
				}
			}
		}

	}
	return concentration;
}

void WordAgent::updateConcentration()
{
	/*gain neighbour*/
	int sum = 0;
	vector<WordAgent * > nearAgents;
	if(env->getNearbyAgents(this, nearAgents))
	{
		for(size_t i = 0; i < nearAgents.size(); i++)
		{
			if(nearAgents[i]->getStatus() != DIE)
			{
				if(ID == nearAgents[i]->getID())
				{
					sum++;
				}
			}
		}
	}
	concentration = sum;
}

void WordAgent::_mapStatusToBehavior()
{
	if(category == ANTIGEN)
	{
		/*selecting behavior according to status of antigen*/

		switch(status)
		{
			case ACTIVE:
				orders.push(MOVING);
				break;
			case DIE:
				orders.push(DYING);
				break;
			default:
				assert(0);
		}
	}
	else
	{
		srand(time(NULL));
		/*selecting behavior according to status of B cell*/
		int n = rand()%3;
		switch(status)
		{
			case ACTIVE:		
				if(n == 0)
				{
					orders.push(MOVING);
				}
				else if(n == 1)
				{
					orders.push(INTERACTING);
				}
				else if(n == 2)
				{
					orders.push(REGULATING);
				}
				break;
			case MATCH:
				orders.push(MUTATING);
				break;
			case MUTATE:
				orders.push(SELECTING);
				break;
			case MATURE:
				orders.push(CLONING);
				break;
			case DIE:
				orders.push(DYING);
				break;
			default:
				assert(0);
		}
	}
}
