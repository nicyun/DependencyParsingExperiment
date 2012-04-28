#ifndef __TRAINER__
#define __TRAINER__

#include "Model.hpp"
#include "Sentence.hpp"
#include "Environment.hpp"
#include "FeatureTemplate.hpp"
#include "WordAgent.hpp"

class Trainer{
private:
	Model * pModel;
	std::vector<WordAgent> BCells;
	Environment * pEnv;
	std::map<std::string, int> wordID;
	std::vector<int> wordFreq;
public:
	Trainer(Model * pm);
	~Trainer();
	bool rfTrain(const Sentence & sen, const std::vector<int> & fa);
	bool addBCells(const Sentence & sen, const std::vector<int> & fa);
	bool cloneBCells();
private:
	int _buildBCell(const std::string & word);
};

#endif

