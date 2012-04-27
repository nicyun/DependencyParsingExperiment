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
	std::queue<int> orders;
	double domAffinity;
	double recAffinity;
	double agAffinity;
	double mutatedAffinity;
	int status;
	int category;
	double stimulus;
	double suppression;
	int concentration;
	std::pair<int, double> feedback;
public:
	WordAgent(int id, 
			Environment * environment,
			const std::pair<int, int> & pos, int cat, int con);
	bool run();
	int getID();

	int getCategory();

	int getStatus();
	void setStatus(int s);

	void gainSuppression(double suppress);

	double getAgAffinity();
	double getMutatedAffinity();
	std::pair<int, int> getPosition() const;
	std::map<int, double> getDomReceptor() const;
	std::vector<int> getRecReceptor() const;
	std::pair<int, double> getFeedback() const;

	int getConcentration();
	void updateConcentration();

	
private:
	bool _doMove();
	bool _interact();
	bool _mutate();
	bool _select();
	bool _clone();
	bool _regulate();
	bool _die();

	/*sence*/
	
	bool _getRegulation();

	int _calConcentration();


	void _communicate();
	bool _updateSelf();
	bool _calFeedback();

	double _calAffinity(std::vector<int> receptor);
	double _calMutatedAffinity(std::vector<int> receptor);
	double _calSuppressByBcell(std::map<int, double> receptor);
	double _calStimulusByBcell(std::vector<int> receptor);

	bool _cmpFeedback(std::pair<int, double> sp, std::pair<int, double> dp);
	
	void _mapStatusToBehavior();
};

#endif
