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
	std::map<int, double> domFeature;
	std::map<int, double> recFeature;
	Environment * env;
	std::queue<int> orders;
public:
	WordAgent(int id, 
			Environment * environment,
			const std::pair<int, int> & pos);
	bool run();
	int getID();
	std::pair<int, int> getPosition() const;
private:
	bool _doMove();
	bool _interact();
	bool _mutate();
};

#endif
