#include "Model.hpp"

using namespace std;

double Model::wordPairWeight(const Sentence & sen, int pa, int pb)
{
	vector<string> featVec;
	feature.abstractFeature(sen, pa, pb, featVec);
	return sumFeatureWeight(featVec);
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
	if(fMap.find(feat) != fMap.end()){
		int id = fMap.size();
		fMap[feat] = id;
	}
	return fMap[feat];
}
