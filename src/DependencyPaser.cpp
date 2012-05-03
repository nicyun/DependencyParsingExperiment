#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <cassert>
#include <cstring>
#include <cstdlib>

#include "DependencyPaser.hpp"
#include "Parameter.hpp"

using namespace std;

DependencyPaser::DependencyPaser()
{
	pModel = new Model();
	pPredictor = new Predictor((Model *)pModel);
	pEvaluation = new Evaluation((Predictor *) pPredictor,(Model *) pModel);
	pTrainer = new Trainer((Model *) pModel, (Evaluation *) pEvaluation);
}

DependencyPaser::~DependencyPaser()
{
	delete pModel;
	delete pTrainer;
	delete pPredictor;
}

bool DependencyPaser::loadModel(const char * file)
{
	return true;
}

bool DependencyPaser::saveModel(const char * file)
{
	return true;
}

bool DependencyPaser::_readFileAddBCell(const char * file)
{
	ifstream fin(file);
	string line;
	vector<vector<string> > senes;
	while(getline(fin, line)){
		if(line == ""){
			vector<int> father;
			Sentence sen;
			sen.push_back(make_pair("ROOT", "ORG"));
			father.push_back(-1);
			for(size_t i = 0; i < senes.size(); i++){
				sen.push_back(make_pair(senes[i][1], senes[i][3]));
				father.push_back(atoi(senes[i][6].c_str()));
			}
			//cout<<"word pairs in a sentence is "<<sen.size()<<endl;
			pModel->getFeatures(sen,senes,father);
			pTrainer->addBCells(sen, father);
			cout<<".";
			senes.clear();
		}
		else{
			vector<string> item;
			string tmp;
			istringstream sin(line);
			while(sin >> tmp){
				item.push_back(tmp);
				//cout<<tmp<<" ";
			}
			//cout<<endl;
			senes.push_back(item);
		}
	}
	cout<<"Relicating B cells according to word frequences...";
        pTrainer->cloneBCells();
        cout<<"Replicate finished!"<<endl;

	return true;
}

bool DependencyPaser::_readFileTrain(const char * file)
{
	pTrainer->constructBcellNet();
	ifstream fin(file);
	string line;
	vector<vector<string> > senes;
	pModel->initFeatureWeight();
	for(size_t i = 0; i < LEARNTIMES; i++)
	{
                while(getline(fin, line)){
                        if(line == ""){
                                vector<int> father;
                                Sentence sen;
                                sen.push_back(make_pair("ROOT", "ORG"));
                                father.push_back(-1);
                                for(size_t i = 0; i < senes.size(); i++){
                                        sen.push_back(make_pair(senes[i][1], senes[i][3]));
                                        father.push_back(atoi(senes[i][6].c_str()));
                                }

                                pTrainer->rfTrain(sen, father);
                                senes.clear();
                                break;
                        }
                        else{
                                vector<string> item;
                                string tmp;
                                istringstream sin(line);
                                while(sin >> tmp){
                                        item.push_back(tmp);
                                }
                                senes.push_back(item);

                        }
                }
	}

	return true;
}
bool DependencyPaser::trainFile(const char * file)
{
        cout<<"Initilizing B cell Network...";
	_readFileAddBCell(file);
	cout<<"Initilize finished!"<<endl;
	cout<<"Training...";
	_readFileTrain(file);
	cout<<"Train finished!"<<endl;

	return true;
}

bool DependencyPaser::predictFile(const char * testFile, const char * outFile)
{
	ifstream fin(testFile);
	ofstream fout(outFile);
	string line;
	vector<vector<string> > senes;
	while(getline(fin, line)){
		if(line == ""){
			vector<int> father;
			Sentence sen;
			sen.push_back(make_pair("ROOT", "ORG"));
			for(size_t i = 0; i < senes.size(); i++){
				sen.push_back(make_pair(senes[i][1], senes[i][3]));
			}
			predict(sen, father);
			for(size_t i = 0; i < senes.size(); i++){
				senes[i][7] = "-";
				fout << senes[i][0];
				for(size_t j = 1; j < senes[i].size(); j++){
					if(j == 6) fout << "\t" << father[i+1];
					else fout << "\t" << senes[i][j];
				}
				fout << endl;
			}
			fout << endl;
			senes.clear();
		}
		else{
			vector<string> item;
			string tmp;
			istringstream sin(line);
			while(sin >> tmp){
				item.push_back(tmp);
			}
			senes.push_back(item);
		}
	}
	return true;
}

double DependencyPaser::predict(const Sentence & sen, vector<int> & fa)
{
	return pPredictor->predict(sen, fa);
}


