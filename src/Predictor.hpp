#ifndef __PREDICTOR__
#define __PREDICTOR__

#include "Model.hpp"
#include "Sentence.hpp"

class Predictor{
private:
	Model * pModel;
private:
	static const int maxLen = 128;
public:
	Predictor(Model * pm);
	double predict(const Sentence & sen, std::vector<int> & fa);
private:
	bool _buildGraph(const Sentence & sen,
			std::vector<std::vector<double> > & graph);
	double _eisner(const std::vector<std::vector<double> > & graph, 
			std::vector<int> & father);
	bool _decode(
			const double f[maxLen][maxLen][2][2],
			int s, int t, int d, int c, 
			const std::vector<std::vector<double> > & graph, 
			std::vector<int> & father);
};

#endif
