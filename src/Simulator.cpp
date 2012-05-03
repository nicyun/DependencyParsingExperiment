#include <vector>
#include <iostream>

#include "Simulator.hpp"
#include "Parameter.hpp"

using namespace std;

Simulator::Simulator(Environment * environment)
{
	env = environment;
}

bool Simulator::addPWordAgent(WordAgent * pWordAgent)
{
	env->addPWordAgent(pWordAgent);
	return true;
}

bool Simulator::run(const Sentence & sen, const std::vector<int> & fa)
{
        /*reset interating objects*/
	bool hasRun = true;
	std::vector<std::set<WordAgent *> > agents = env->getAgents();
	/*termination consitions:
	(1) All antigens are killed;
	(2) All B cells are active;
	*/

	while(hasRun){
	        hasRun = false;
                for(size_t i = 0; i < agents.size(); i++){
			std::set<WordAgent *>::iterator it = agents[i].begin();

			for(; it != agents[i].end(); it++)
			{
			        //cout<<(*it)->getPosition().first<<","<<(*it)->getPosition().second<<" ";
			        (*it)->run();
				if((env->getAntigenNum() != 0)){
					hasRun = true;
				}

			}
		}
	}

	return true;
}



