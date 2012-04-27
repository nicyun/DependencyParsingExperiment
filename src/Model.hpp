#ifndef __MODEL__
#define __MODEL__

#include <map>
#include <vector>

#include "Sentence.hpp"
#include "FeatureTemplate.hpp"

class Model{
private:
	std::map<std::string, int> fMap;
	std::vector<double> fWeight;
	FeatureTemplate ft;
public:
	double wordPairWeight(const Sentence & sen, int p, int c);
	double sumFeatureWeight(const std::vector<std::string> & featVec);
	bool getFeatureIDVec(const Sentence & sen, int p, int c,
			std::vector<int> & featIDVec);
	int addFeature(const std::string & feat);
private:
	int _getFeatureID(const std::string & feat);
};

#endif
