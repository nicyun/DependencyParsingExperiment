#ifndef __WORD_AGENT__
#define __WORD_AGENT__

#include <string>
#include <map>
#include <vector>
#include <queue>

#include "Environment.hpp"

class WordAgent{
private:
	int ID;
	std::vector<int> attrs;
	const Environment * env;
	std::map<int, double> affinityTo;
	std::map<int, double> affinityFrom;
	std::queue<int> orders;
public:
	bool init(int id, const std::vector<int> & attributes, 
			const Environment * environment);
	bool initOrder();
	double affinityToFather(WordAgent * pFather);
	double affinityToSon(WordAgent * pSon);
	bool getAttributes(std::vector<int> & attr);
	bool run();
	int getID();
	std::map<int, double> & getAffinityTo();
	std::map<int, double> & getAffinityFrom();
private:
	bool _calcAffinities();
};

#endif
