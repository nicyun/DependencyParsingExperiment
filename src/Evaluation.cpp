#include "Evaluation.hpp"

using namespace std;

Evaluation::Evaluation()
{}

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
