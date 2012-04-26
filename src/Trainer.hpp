#ifndef __TRAINER__
#define __TRAINER__

#include "Model.hpp"
#include "Sentence.hpp"
#include "Simulator.hpp"
#include "Environment.hpp"

class Trainer{
private:
	Model * pModel;
	Environment * env;
	Simulator * sim;
	std::vector<std::pair<WordAgent *, int> > antigens;
public:
	Trainer(Model * pm);
	bool rfTrain(const Sentence & sen, const std::vector<int> & fa);
private:
	bool _constructAntigen();
	
};

#endif

