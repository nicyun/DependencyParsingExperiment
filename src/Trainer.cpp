#include "Trainer.hpp"
#include "Parameter.hpp"

#include <iostream>
#include <vector>

using namespace std;

Trainer::Trainer(Model * pm) : pModel(pm)
{
	pEnv = new Environment(ROWS, COLS);
}

Trainer::~Trainer()
{
	delete pEnv;
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/
	/*construct antigens*/

	/*immune tuning*/

	return true;
}

bool Trainer::addBCells(const Sentence & sen, const vector<int> & fa)
{
	vector<int> features;
	for(size_t i = 1; i < sen.size(); i++){
		int j = fa[i];
		int bi = _buildBCell(sen[i].first);
		int bj = _buildBCell(sen[j].first);
		pModel->getFeatureIDVec(sen, j, i, features);
		BCells[bi].addTmpFeature(features);
		BCells[bj].addDomFeature(features);
	}
	return true;
}

bool Trainer::cloneBCells()
{
	for(size_t i = 0; i < wordFreq.size(); i++){
		for(int j = 1; j < wordFreq[i]; j++){
			BCells.push_back(BCells[i]);
		}
	}
	return true;
}

int Trainer::_buildBCell(const string & word)
{
	if(wordID.find(word) == wordID.end()){
		wordID[word] = BCells.size();
		BCells.push_back(WordAgent(wordID[word], pEnv, 
				pEnv->getRandomPosition()));
	}
	int res = wordID[word];
	if((int)wordFreq.size() <= res){
		wordFreq.push_back(0);
	}
	wordFreq[res] ++;
	return res;
}
