#ifndef __DEPENDENCY_PASER__
#define __DEPENDENCY_PASER__

#include <map>

#include "Trainer.hpp"
#include "Predictor.hpp"
#include "Model.hpp"
#include "Sentence.hpp"

class DependencyPaser{
private:
	Model * pModel;
	Trainer * pTrainer;
	Predictor * pPredictor;
public:
	DependencyPaser();
	~DependencyPaser();
	bool loadModel(const char * file);
	bool saveModel(const char * file);
	bool trainFile(const char * file);
	bool rfTrain(const Sentence & sen, const std::vector<int> & fa);
	double predict(const Sentence & sen, std::vector<int> & fa);
	bool predictFile(const char * testFile, const char * outFile);
private:
};

#endif
