#ifndef _EVALUATION_H
#define _EVALUATION_H

#include <vector>

#include "Predictor.hpp"
#include "Model.hpp"
#include "WordAgent.hpp"

class WordAgent;

class Evaluation{
private:
	Predictor * pPredictor;
	Model * pModel;
public:
	Evaluation(Predictor * pre, Model * mod);
	~Evaluation();

	double calAccuracy(const std::vector<int> & newLabel,
			const std::vector<int> & orgLabel);
	std::pair<int, double> calFeedback(const Sentence & sen,WordAgent * wa, std::vector<int> & standard);
};

#endif
