#include "Trainer.hpp"
#include "Parameter.hpp"

#include <iostream>
#include <vector>

using namespace std;

Trainer::Trainer(Model * pm, Evaluation * eva) : pModel(pm)
{
	pEnv = new Environment(ROWS, COLS,eva);
	simu = new Simulator(pEnv);
}

Trainer::~Trainer()
{
	delete pEnv;
	delete simu;
}

bool Trainer::rfTrain(const Sentence & sen, const vector<int> & fa)
{
	/*update weights of receptor(features) by learning from a sample*/
	/*construct antigens*/
	for(size_t i = 1; i < sen.size();)
	{
	        _addAntigen(sen, fa,i);
		if(simu->run(sen,fa))
		{
			i++;
		}
	}
	return true;
}

bool Trainer::addBCells(const Sentence & sen, const vector<int> & fa)
{
	vector<int> features;
	for(size_t i = 1; i < sen.size(); i++){
		int j = fa[i];
		int bi = _buildBCell(sen[i].first);
		int bj = _buildBCell(sen[j].first);
		pModel->getFeatureIDVec(sen, j, i, features);
		BCells[bi].addRecFeature(features);
		BCells[bj].addDomFeature(features);
	}
	return true;
}

int Trainer::_buildBCell(const string & word)
{
        if(wordID.find(word) == wordID.end()){
		wordID[word] = BCells.size();
		bool flag = true;
		while(flag)
		{
                        pair<int,int> pos = pEnv->getRandomPosition();
                        if(pEnv->getLocalAgentsNum(pos) < MAXNUMAGENT)/*local number of agents must be lower than threshold*/
                        {
                                pEnv->setLocalAgentsNum(pos);
                                BCells.push_back(WordAgent(wordID[word], pEnv,
                                                pos, BCELL,1));
                                flag = false;
                        }
		}
	}
	int res = wordID[word];
	if((int)wordFreq.size() <= res){
		wordFreq.push_back(0);
	}
	wordFreq[res] ++;
	return res;
}

bool Trainer::constructBcellNet()
{
        cout<<"Constructing B cell network..."<<endl;
	for(size_t i = 0; i < BCells.size(); i++)
	{
		simu->addPWordAgent(&BCells[i]);
	}
	cout<<"Construct finished!"<<endl;
	return true;
}

int Trainer::_buildAntigen(const string & word)
{

	Antigens.push_back(WordAgent(wordID[word],pEnv,pEnv->getRandomPosition(), ANTIGEN,1));
	return wordID[word];
}

bool Trainer::_addAntigenToSimulator(const Sentence & sen, const std::vector<int> & fa)
{

	return true;
}

bool Trainer::_addAntigen(const Sentence & sen, const std::vector<int> & fa,int i)
{
        vector<int> features;
        int j = fa[i];
        int bi = _buildAntigen(sen[i].first);
        int bj = _buildAntigen(sen[j].first);
        pModel->getFeatureIDVec(sen,j,i,features);
        Antigens[bi].addRecFeature(features);
        //cout<<"adding antigen "<<i<<endl;
        vector<vector<int> > v =  pEnv->getGrid();
        vector<WordAgent> vwa;
        for(size_t i = 0; i <  AGQUANTITY; i++)
        {
                WordAgent wa(Antigens[bi].getID(), pEnv,Antigens[bi].getPosition(), ANTIGEN,1);
                map<int,double> ma = Antigens[bi].getDomReceptor();
                wa.setDomReceptor(ma);
                vector<int> v = Antigens[bi].getRecReceptor();
                wa.setRecReceptor(v);
                vwa.push_back(wa);
        }

        int c = 0;
        while(c < AGQUANTITY)
        {
                for(size_t m = 0; m < v.size(); m++)
                {
                        for(size_t n = 0; n < v[m].size(); n++)
                        {
                                if(v[m][n] != 0)
                                {
                                        if(c < AGQUANTITY)
                                        {
                                                vwa[c].setPosition(make_pair(m,n));
                                                simu->addPWordAgent(&vwa[c]);
                                                c++;
                                        }
                                        else
                                        {
                                                break;
                                        }
                                }
                        }
                }
        }

	return true;

}

bool Trainer::cloneBCells()
{
	for(size_t i = 0; i < wordFreq.size(); i++){
		for(int j = 1; j < wordFreq[i]; j++){

                        if(pEnv->getLocalAgentsNum(BCells[i].getPosition()) < MAXNUMAGENT)
                        {
                                pEnv->setLocalAgentsNum(BCells[i].getPosition());
                                WordAgent wa(BCells[i].getID(), pEnv,BCells[i].getPosition(), BCELL,1);
                                map<int,double> m = BCells[i].getDomReceptor();
                                wa.setDomReceptor(m);
                                vector<int> v = BCells[i].getRecReceptor();
                                wa.setRecReceptor(v);
                                BCells.push_back(wa);
                        }
                        else
                        {
                                bool flag = true;
                                while(flag)
                                {
                                        pair<int,int> pos = pEnv->getRandomPosition();
                                        if(pEnv->getLocalAgentsNum(pos) < MAXNUMAGENT)/*local number of agents must be lower than threshold*/
                                        {
                                                pEnv->setLocalAgentsNum(pos);

                                                WordAgent wa(BCells[i].getID(), pEnv,pos, BCELL,1);
                                                map<int,double> m = BCells[i].getDomReceptor();
                                                wa.setDomReceptor(m);
                                                vector<int> v = BCells[i].getRecReceptor();
                                                wa.setRecReceptor(v);
                                                BCells.push_back(wa);
                                                flag = false;
                                        }
                                }
                        }
		}
		cout<<".";
	}
	return true;
}

void Trainer::testSub()
{
        pEnv->testSub(20);
}

void Trainer::testAgentNum()
{
        vector<set<WordAgent *> > w = pEnv->getAgents();
        for(size_t j = 0; j < w.size(); j++)
        {
                cout<<w[j].size()<<" ";
        }
        cout<<endl;
}
