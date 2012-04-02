#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <map>

class Environment{
private:
	std::map<std::pair<int, int>, double> wordStat;
	std::map<std::pair<int, int>, double> posStat;
	std::vector<class WordAgent *> pWordAgents;
public:
	bool addWordStat(int father, int son, double value);
	bool addPosStat(int father, int son, double value);
	bool addPWordAgent(class WordAgent * pWordAgent);
	// calculation is directed
	double calcAffinity(const std::vector<int> & attrs1,
			const std::vector<int> & attrs2) const;
	bool clearAgents();
	bool getNearbyAgents(std::vector<class WordAgent *> & neabyAgents) const;
};

#endif
