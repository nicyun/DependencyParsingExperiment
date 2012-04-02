#ifndef __DEPENDENCY_PASER__
#define __DEPENDENCY_PASER__

#include <map>

#include "Simulator.hpp"
#include "WordAgent.hpp"
#include "Environment.hpp"

class DependencyPaser{
private:
	Environment env;
	std::map<std::string, int> strMap;
public:
	bool loadModel(const char * file);
	double predict(const std::vector<std::string> & words,
			const std::vector<std::string> & postags,
			std::vector<int> & fathers);
	bool predictFile(const char * testFile, const char * outFile);
private:
	int _getWordID(const std::string & word);
	bool _buildGraph(const std::vector<std::string> & words,
			const std::vector<std::string> & postags, 
			std::vector<std::vector<double> > & graph);
	double _eisner(const std::vector<std::vector<double> > & graph, 
			std::vector<int> & father);
	bool _decode(
			const std::vector<std::vector<std::vector<std::vector<double> > > > & f, 
			int s, int t, int d, int c, 
			const std::vector<std::vector<double> > & graph, 
			std::vector<int> & father);
};

#endif
