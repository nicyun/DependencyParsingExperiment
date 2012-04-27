#include "Trainer.hpp"
#include "Parameter.hpp"

#include <iostream>
#include <stdlib.h>

using namespace std;

Trainer::Trainer(Model * pm) : pModel(pm)
{
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/
	/*construct antigens:initailizing receptors and the number of agents*/
	if(_constructAntigen(sen))
	{
		/*injecting antigens randomly by environment:
		selecting positions randomly
		*/
		for(size_t i = 0; i < antigens.size();)
		{
			for(int j = 0; j < antigens[i].second; j++ )
			{
				env->addPWordAgent(antigens[i].first);
			}
			/*immune regulating:
			when all antigens are died, and clone is happened, current regulation is finished!
			*/
			if(sim->run())
			{
				i++;
			}
		}
	}

	return true;
}

bool Trainer::_constructAntigen(const Sentence & sen)
{

	return true;
}
