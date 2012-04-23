#include "Trainer.hpp"
#include "Parameter.hpp"

#include <iostream>
#include <stdlib.h>

using namespace std;

Trainer::Trainer(Model * pm) : pModel(pm)
{
	if(_initailizeWordAgentNetwork())
	{
		std::cerr<<"Initializing word-agent network failed!"<<std::endl;
		exit(1);
	}
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/
	/*construct antigens*/

	/*immune tuning*/

	return true;
}

bool Trainer::_initailizeWordAgentNetwork()
{
	return true;
}
