#ifndef _INFORMATION_H_
#define _INFORMATION_H_

#include "WordAgent.hpp"

struct Table{
	int featureID;
	double weight;
	int feedback;
	double deta;
	int sc;
};

class Information{
private:
	std::vector<Table> vt;
public:
	Information();
    	~Information();

    	std::pair<int, double> getFeedback(WordAgent * pWordAgent);
	std::map<int, double> getInfor(WordAgent * pWordAgent);
private:

};

#endif
