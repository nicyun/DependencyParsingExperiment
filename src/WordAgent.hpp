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
	std::vector<int> agFeature;
	std::vector<int> mutatePosition;
	Environment * env;
	double domAffinity;
	double recAffinity;
	double agAffinity;
	int status;
	int category;
	double stimulus;
	double suppression;
	int concentration;
	std::pair<int, double> feedback;
public:
	WordAgent(int id, 
			Environment * environment,
			const std::pair<int, int> & pos);
	bool run();
	int getID();
	bool addDomFeature(const std::vector<int> & feature);
	bool addTmpFeature(const std::vector<int> & feature);

	int getCategory();

	int getStatus();
	void setStatus(int s);

	void gainSuppression(double suppress);

	double getAgAffinity();
	std::pair<int, int> getPosition() const;
	std::map<int, double> getDomReceptor() const;
	std::vector<int> getRecReceptor() const;
	std::pair<int, double> getFeedback() const;

	bool select();
private:
	bool _doMove();
	bool _interact();
	bool _mutate();
	bool _clone();
	bool _regulate();
	bool _die();

	/*sence*/
	bool _getStimulus();
	bool _getSuppression();
	bool _getRegulation();

	void _communicate();
	bool _updateSelf();
	bool _calFeedback();

	double _calAffinity(std::vector<int> receptor);
	double _calMutatedAffinity(std::vector<int> receptor);
	double _calAffinity(std::map<int, double> receptor);

	bool _cmpFeedback(std::pair<int, double> sp, std::pair<int, double>dp);

};

#endif
