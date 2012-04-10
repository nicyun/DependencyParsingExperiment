#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <map>
#include <set>

class WordAgent;

class Environment{
private:
	std::map<std::pair<int, int>, double> wordStat;
	std::map<std::pair<int, int>, double> posStat;
	std::vector<std::set<WordAgent *> > pWordAgents;
	int rows, cols;
	int numClone;
public:
	Environment(int r, int c, int cnt);
	bool addWordStat(int father, int son, double value);
	bool addPosStat(int father, int son, double value);
	bool addPWordAgent(WordAgent * pWordAgent);
	bool delPWordAgent(WordAgent * pWordAgent);
	// calculation is directed
	double calcAffinity(const std::vector<int> & attrs1,
			const std::vector<int> & attrs2) const;
	bool resetAgents();
	bool getNearbyAgents(const WordAgent * pWordAgent,
			std::vector<WordAgent *> & neabyAgents) const;
	int getNumClone();
	int agentCount(const std::pair<int, int> & pos);
	std::pair<int, int> getRandomPosition();
	bool xInRange(int x);
	bool yInRange(int y);
private:
	int _calcSub(const std::pair<int, int> & pos) const;
};

#endif
