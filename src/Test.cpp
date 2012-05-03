#include "WordAgent.hpp"
#include "Environment.hpp"
#include "Parameter.hpp"
#include "DependencyPaser.hpp"

#include <iostream>

using namespace std;

void testWordAgentInit()
{
	std::pair<int, int> pos;
	pos.first = 3;
	pos.second = 3;
	/*Environment * env = new Environment(4,4);
	WordAgent * wa = new WordAgent(10,env,pos,BCELL,1);
//	cout<<"add agent"<<endl;
	env->addPWordAgent(wa);
//	cout<<"move"<<endl;

//	cout<<"new pos "<<wa->getPosition().first<<","<<wa->getPosition().second<<endl;
	map<int, double> m = wa->getDomReceptor();
	map<int,double>::iterator it;
//	cout<<"domFeature is "<<endl;
	WordAgent * wb = new WordAgent(11,env,pos,ANTIGEN,1);
	env->addPWordAgent(wb);
	WordAgent * wd = new WordAgent(13,env,pos,ANTIGEN,1);
	env->addPWordAgent(wd);

	WordAgent * wc = new WordAgent(12,env,pos,BCELL,1);
	env->addPWordAgent(wc);

	int count = env->agentCount(pos);
	cout<<"number of agents is "<<count<<endl;


	wa->run();
	cout<<"id is "<<wa->getID()<<endl;
	count = env->agentCount(pos);
	cout<<"number of agents is "<<count<<endl;
	wc->run();
	cout<<"id is "<<wc->getID()<<endl;
	count = env->agentCount(pos);
	cout<<"number of agents is "<<count<<endl;

	cout<<"status "<<wa->getStatus()<<","<<wc->getStatus()<<endl;
	for(it = m.begin(); it != m.end(); it++)
	{
		cout<<(*it).first<<","<<(*it).second<<endl;
	}

	cout<<"recFeature is "<<endl;
	vector<int> v = wa->getRecReceptor();
	vector<int>::iterator t;
	for(t = v.begin(); t != v.end(); t++)
	{
		cout<<(*t)<<endl;
	}


	cout<<"delete env"<<endl;
	delete env;
	cout<<"delete agent"<<endl;
	delete wa;
	delete wb;
	delete wd;
	delete wc;
*/
}

void testTrain()
{
        DependencyPaser dp;
        dp.trainFile("./data/english_train-copy.conll");
}

void testCulSub()
{


}
