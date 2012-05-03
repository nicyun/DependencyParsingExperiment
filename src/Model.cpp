#include "Model.hpp"
#include <iostream>
using namespace std;


double Model::wordPairWeight(const Sentence & sen, int p, int c)
{
	vector<string> featVec;
	ft.abstractFeature(sen, p, c, featVec);
	return sumFeatureWeight(featVec);
}

bool Model::getFeatureIDVec(const Sentence & sen, int p, int c,
		vector<int> & featIDVec)
{
	vector<string> featVec;
	ft.abstractFeature(sen, p, c, featVec);
	featIDVec.clear();
	for(size_t i = 0; i < featVec.size(); i++){
		featIDVec.push_back(addFeature(featVec[i]));
	}
	return true;
}

double Model::sumFeatureWeight(const vector<string> & featVec)
{
	double sum = 0;
	for(size_t i = 0; i < featVec.size(); i++){
		int fid = _getFeatureID(featVec[i]);
		if(fid == -1) continue;
		sum += fWeight[fid];
	}
	return sum;
}

int Model::_getFeatureID(const string & feat)
{
	if(fMap.find(feat) == fMap.end()) return -1;
	return fMap[feat];
}

int Model::addFeature(const string & feat)
{
	if(fMap.find(feat) == fMap.end()){
		int id = fMap.size();
		fMap[feat] = id;
	}

	return fMap[feat];
}

bool Model::getFeatures(const Sentence & sen,vector<vector<string> > &sens, vector<int> & fa)
{
	for(size_t j = 1; j < sens.size(); j++)
	{
		vector<string> fea;
		ft.abstractFeature(sen, fa[j], j, fea);
	}

	return true;
}

void Model::initFeatureWeight()
{
	fWeight.resize((int)fMap.size());
}

vector<double> Model::getFeatureWeight()
{
	return fWeight;
}

bool Model::setFeatureWeight(std::vector<double> & newWeight)
{
        fWeight = newWeight;
        return true;
}

bool Model::updateFeatureWeight(map<int, double> & domFeatures)
{
	map<int, double>::iterator it;
	for(it = domFeatures.begin(); it != domFeatures.end(); it++)
	{
		if(it->first < (int)fWeight.size())
		{
			fWeight[it->first] = it->second;
		}
	}
	return true;
}
