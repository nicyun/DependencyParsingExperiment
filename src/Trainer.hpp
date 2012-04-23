#ifndef __TRAINER__
#define __TRAINER__

#include "Model.hpp"
#include "Sentence.hpp"
#include "Environment.hpp"

class Trainer{
private:
	Model * pModel;
public:
	Trainer(Model * pm);
	bool rfTrain(const Sentence & sen, const std::vector<int> & fa);
private:
	bool _initailizeWordAgentNetwork();
};

#endif

