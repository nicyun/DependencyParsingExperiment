#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <map>
#include <set>

class WordAgent;

class Environment{
private:
	std::vector<std::set<WordAgent *> > pWordAgents;
	int rows, cols;
public:
	Environment(int r, int c);
	bool addPWordAgent(WordAgent * pWordAgent);
	bool delPWordAgent(WordAgent * pWordAgent);
	// calculation is directed
	bool resetAgents();
	bool getNearbyAgents(const WordAgent * pWordAgent,
			std::vector<WordAgent *> & neabyAgents) const;
	int agentCount(const std::pair<int, int> & pos);
	std::pair<int, int> getRandomPosition();
	bool xInRange(int x);
	bool yInRange(int y);
	bool updateLocalEnv(WordAgent * pWordAgent);
private:
	int _calcSub(const std::pair<int, int> & pos) const;
};

#endif
