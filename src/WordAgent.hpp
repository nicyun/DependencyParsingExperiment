#ifndef __WORD_AGENT__
#define __WORD_AGENT__

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <set>

#include "Environment.hpp"

class WordAgent{
private:
	int ID;
	std::pair<int, int> position;
	std::vector<int> attrs;
	Environment * env;
	std::map<int, double> affinityTo;
	std::map<int, double> affinityFrom;
	std::queue<int> orders;
public:
	WordAgent(int id, 
			const std::vector<int> & attributes, 
			Environment * environment,
			const std::pair<int, int> & pos);
	bool initOrder();
	double affinityFromFather(WordAgent * pFather);
	double affinityToSon(WordAgent * pSon);
	bool getAttributes(std::vector<int> & attr);
	bool runByOrder();
	bool run();
	int getID();
	std::map<int, double> & getAffinityTo();
	std::map<int, double> & getAffinityFrom();
	std::pair<int, int> getPosition() const;
private:
	bool _calcAffinities();
	bool _doMove();
};

#endif
