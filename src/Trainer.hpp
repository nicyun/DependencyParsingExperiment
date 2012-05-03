#ifndef __TRAINER__
#define __TRAINER__

#include "Model.hpp"
#include "Sentence.hpp"
#include "Environment.hpp"
#include "FeatureTemplate.hpp"
#include "WordAgent.hpp"
#include "Simulator.hpp"

class Trainer{
private:
	Model * pModel;
	std::vector<WordAgent> BCells;
	Environment * pEnv;
	std::map<std::string, int> wordID;
	Simulator * simu;
	std::vector<WordAgent> Antigens;
	std::vector<int> wordFreq;
public:
	Trainer(Model * pm, Evaluation * eva);
	~Trainer();
	bool rfTrain(const Sentence & sen, const std::vector<int> & fa);
	bool addBCells(const Sentence & sen, const std::vector<int> & fa);
	bool constructBcellNet();
        bool cloneBCells();

        void testSub();
        void testAgentNum();
private:
	int _buildBCell(const std::string & word);
	bool _addAntigenToSimulator(const Sentence & sen, const std::vector<int> & fa);
	int _buildAntigen(const std::string & word);
	bool _addAntigen(const Sentence & sen, const std::vector<int> & fa,int i);
};

#endif

