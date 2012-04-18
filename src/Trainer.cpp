#include "Trainer.hpp"
#include "Parameter.hpp"

using namespace std;

Trainer::Trainer(Model * pm) : pModel(pm) 
{
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/

	return true;
}

bool Trainer::onlineLearning(const std::vector<Sentence> & vsen, const std::vector<std::vector<int> > & vfa)
{
	/*online learning by immune rl*/
	for(size_t i = 0; i < LEARNTIMES; i++)
	{
		/*immune rl*/
		for(size_t j = 0; j < vsen.size(); j++)
		{
			/*immune rl from a sentence*/
			rfTrain(vsen[j],vfa[j]);
		}
	}
	return true;
}
