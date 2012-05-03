#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <vector>
#include <map>
#include <set>

#include "Sentence.hpp"
#include "Predictor.hpp"
#include "Evaluation.hpp"
class WordAgent;
class Information;
class Evaluation;

class Environment{
private:
	std::vector<std::set<WordAgent *> > pWordAgents;
	int rows, cols;
	Information * infor;
	Sentence sen;
	std::vector<int> fa;
	Evaluation  * eva;
	int pAntigenNum;
	std::vector<std::vector<int> > grid;

	std::vector<int> badrow;
	std::vector<int> badcol;
public:
	Environment(int r, int c, Evaluation * evaluation);
	bool addPWordAgent(WordAgent * pWordAgent);
	bool delPWordAgent(WordAgent * pWordAgent);
	// calculation is directed
	bool resetAgents();
	bool getNearbyAgents(const WordAgent * pWordAgent,
			std::vector<WordAgent *> & neabyAgents) const;
	std::vector<std::set<WordAgent *> > getAgents();
	int agentCount(const std::pair<int, int> & pos);
	std::pair<int, int> getRandomPosition();
	bool xInRange(int x);
	bool yInRange(int y);
	bool update(WordAgent * pWordAgent);
	std::map<int, double> getInfor(WordAgent * pWordAgent);
	std::pair<int, double> gainFeedback(WordAgent * pWordAgent);
	void gainSentenceInfor(const Sentence & sentence,const  std::vector<int> & father);
	int getAntigenNum();
	int getLocalAgentsNum(std::pair<int,int> pos);
	void setLocalAgentsNum(std::pair<int,int> pos);
	std::vector<std::vector<int> > getGrid();
	void testSub(int a);
private:
	int _calcSub(const std::pair<int, int> & pos) const;
	void _initGrid();
};



#endif
