#include "Information.hpp"

Information::Information()
{}

Information::~Information()
{}

bool Information::getFeedback(WordAgent * pWordAgent)
{
	return true;
}

std::map<int, double> Information::getInfor(WordAgent * pWordAgent)
{
	std::map<int, double> infor;
	/*obtain information of receptor in the environment*/
	std::map<int, double> domRec = pWordAgent->getDomReceptor();
	std::map<int, double>::iterator it;
	std::vector<Table>::iterator tmp;
	for(it = domRec.begin(); it != domRec.end(); it++)
	{
		for(tmp = vt.begin(); tmp != vt.end(); tmp++)
		{
			if(tmp->featureID == it->first)
			{
				infor.insert(std::map<int, double>::value_type(tmp->featureID, tmp->weight));
				break;
			}
		}
	}

	return infor;
}
