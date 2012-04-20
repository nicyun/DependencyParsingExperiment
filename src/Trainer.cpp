#include "Trainer.hpp"
#include "Parameter.hpp"

using namespace std;

Trainer::Trainer(Model * pm) : pModel(pm)
{
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/
	/*construct antigens*/

	/*immune tuning*/

	return true;
}
