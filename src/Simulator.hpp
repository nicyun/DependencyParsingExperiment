#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <vector>

#include "WordAgent.hpp"
#include "Sentence.hpp"

class Simulator{
private:
	Environment * env;
public:
	Simulator(Environment * environment);
	bool addPWordAgent(WordAgent * pWordAgent);
	bool run(const Sentence & sen, const std::vector<int> & fa);
private:
        bool resetInteratObjects();
};

#endif
