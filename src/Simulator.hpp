#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <vector>

#include "WordAgent.hpp"

class Simulator{
private:
	std::vector<WordAgent *> pWordAgents;
public:
	bool addPWordAgent(WordAgent * pWordAgent);
	bool run();
};

#endif
