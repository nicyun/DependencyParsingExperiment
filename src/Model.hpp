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
	FeatureTemplate feature;
public:
	double wordPairWeight(const Sentence & sen, int pa, int pb);
	double sumFeatureWeight(const std::vector<std::string> & featVec);
	int addFeature(const std::string & feat);
private:
	int _getFeatureID(const std::string & feat);
};

#endif
