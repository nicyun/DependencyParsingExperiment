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
	if(_constructAntigen())
	{
		/*injecting antigens randomly by environment:
		selecting positions randomly
		*/
		for(size_t i = 0; i < antigens.size();)
		{
			int n = antigens[i].second;
			for(int j = 0; j < n; j++ )
			{
				pair<int,int> pos;
				int id = 0;
				WordAgent * wa = new WordAgent(id, env, pos, ANTIGEN, 1);
				env->addPWordAgent(wa);
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

bool Trainer::_constructAntigen()
{

	return true;
}
