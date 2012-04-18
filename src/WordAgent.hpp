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
	std::vector<int> recFeature;
	Environment * env;
	std::queue<int> orders;
	double domAffinity;
	double recAffinity;
	int status;
	int stimulus;
	int suppression;
	int concentration;
public:
	WordAgent(int id, 
			Environment * environment,
			const std::pair<int, int> & pos);
	bool run();
	int getID();
	std::pair<int, int> getPosition() const;

	bool interact(std::vector<int> receptor);
	bool select();
private:
	bool _doMove();
	bool _mutate(std::vector<int> mutatePosi, std::map<int,double> & f);
	bool _clone();
	bool _regulate();

	double _calAffinity(std::vector<int> receptor);
	int _getStatus();
	void _setStatus(int s);

};

#endif
