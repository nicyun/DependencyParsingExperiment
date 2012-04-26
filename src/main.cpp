#include <iostream>
#include <cassert>

#include "DependencyPaser.hpp"
#include "Test.hpp"

using namespace std;

int main(int argc, char * argv[])
{
/*	assert(argc == 4);
	DependencyPaser dp;
	cout<<"load model"<<endl;
	dp.loadModel(argv[1]);
	cout << "model loaded!" << endl;
	dp.predictFile(argv[2], argv[3]);
	cout << "predicted OK" << endl;
*/
	testWordAgentInit();
	return 0;
}
