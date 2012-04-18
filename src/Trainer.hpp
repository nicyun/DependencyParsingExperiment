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
	bool onlineLearning(const std::vector<Sentence> & vsen, const std::vector<std::vector<int> > & vfa );
};

#endif

