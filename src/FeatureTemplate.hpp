#ifndef __FEATURETMPLATE__
#define __FEATURETMPLATE__

#include <vector>
#include <string>

#include "Sentence.hpp"

class FeatureTemplate{
private:
	const Sentence * pSen;
	int p, c;
public:
	bool abstractFeature(const Sentence & sen, int pa, int pb,
			std::vector<std::string> & featVec);
private:
	bool _abstractFeature(std::vector<std::string> & featVec);
	std::string _crtWord(int pos);
	std::string _crtPos(int pos);
	std::string _nextPos(int pos);
	std::string _prePos(int pos);
	bool _initHeader(std::string & str, int pos);
};

#endif

