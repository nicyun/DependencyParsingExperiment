#ifndef _EVALUATION_H
#define _EVALUATION_H

#include <vector>

class Evaluation{
private:
public:
	Evaluation();
	~Evaluation();

	double calAccuracy(const std::vector<int> & newLabel,
			const std::vector<int> & orgLabel);
};

#endif
