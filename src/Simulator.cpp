#include <vector>

#include "Simulator.hpp"

using namespace std;

bool Simulator::addPWordAgent(WordAgent * pWordAgent)
{
	pWordAgents.push_back(pWordAgent);
	return true;
}

bool Simulator::run()
{
	bool hasRun = true;
	while(hasRun){
		hasRun = false;
		for(size_t i = 0; i < pWordAgents.size(); i++){
			if(pWordAgents[i]->run()){
				hasRun = true;
			}
		}
	}
	return true;
}


