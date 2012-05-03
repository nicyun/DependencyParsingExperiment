#include "Evaluation.hpp"
#include "Parameter.hpp"

using namespace std;

Evaluation::Evaluation(Predictor * pre, Model * mod)
{
	pPredictor = pre;
	pModel = mod;
}

Evaluation::~Evaluation()
{}

double Evaluation::calAccuracy(const vector<int> & newLabel,
		const vector<int> & orgLabel)
{
	double accuracy = 0;
	for(size_t i = 0; i < newLabel.size(); i++){
		accuracy += (newLabel[i] == orgLabel[i]);
	}
	return accuracy / newLabel.size();
}

pair<int, double>  Evaluation::calFeedback(const Sentence & sen, WordAgent * wa, vector<int> & standard)
{
	vector<double> tmp;
	tmp = pModel->getFeatureWeight();
	vector<int> father;
	double value = pPredictor->predict(sen,father);
	double accuracy = calAccuracy(father, standard);
	map<int, double> domFeature = wa->getTmpReceptor();
	pModel->updateFeatureWeight(domFeature);
	vector<int> mutatefather;
	double mutatevalue = pPredictor->predict(sen,mutatefather);
	double mutateaccuracy = calAccuracy(mutatefather, standard);
	int differ = int((mutateaccuracy - accuracy) * PRECISION);
	pair<int, double> p;
	if((differ == 0) && (accuracy == 1.0))
	{
		int d = int((mutatevalue - value) * PRECISION);
		if(d > 0)
		{
			p.first = 2;
			p.second = mutatevalue;
			return p;
		}
	}
	else if(differ > 0)
	{
		p.first = 1;
		p.second = mutateaccuracy;
		return p;
	}

	p.first = -1;
	p.second =  0.0;
	pModel->setFeatureWeight(tmp);

	return p;

}
