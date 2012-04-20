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
	std::map<int, double> tmpFeature;
	std::vector<int> recFeature;
	std::vector<int> mutatePosition;
	Environment * env;
	std::queue<int> orders;
	double domAffinity;
	double recAffinity;
	double agAffinity;
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

	int getStatus();
	void setStatus(int s);

	double getAgAffinity();
	std::pair<int, int> getPosition() const;
	std::map<int, double> getDomReceptor() const;
	std::vector<int> getRecReceptor() const;

	bool interact(std::vector<int> receptor);
	bool select();
private:
	bool _doMove();
	bool _mutate();
	bool _clone();
	bool _regulate();
	bool _die();

	/*sence*/
	bool _getStimulus();
	bool _getSuppression();

	void _communicate();
	bool _getFeedback();

	double _calAffinity(std::vector<int> receptor);
	double _calAffinity(std::map<int, double> receptor);

};

#endif
