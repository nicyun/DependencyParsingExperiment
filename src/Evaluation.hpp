#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Sentence.hpp"

class Evaluation{
private:
	double accuracy;

public:
	Evaluation();
	~Evaluation();

	double calAccuracy(Sentence & s);
};

#endif
