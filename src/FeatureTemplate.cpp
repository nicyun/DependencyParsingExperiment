#include "FeatureTemplate.hpp"

using namespace std;

bool FeatureTemplate::abstractFeature(const Sentence & sen,
		int parent, int child, vector<string> & featVec)
{
	pSen = &sen;
	p = parent;
	c = child;
	return _abstractFeature(featVec);
}

bool FeatureTemplate::_abstractFeature(vector<string> & featVec)
{
	featVec.clear();
	// p-word
	featVec.push_back(_crtWord(p));
	// c-word
	featVec.push_back(_crtWord(c));
	// p-word,c-word
	featVec.push_back(_crtWord(p) + "_" + _crtWord(c));
	// p-word,c-word,c-pos
	featVec.push_back(_crtWord(p) + "_" + _crtWord(c) + "_" + _crtPos(c));
	// p-word,p-pos,c-word
	featVec.push_back(_crtWord(p) + "_" + _crtPos(p) + "_" + _crtWord(c));
	// p-word,p-pos,c-pos
	featVec.push_back(_crtWord(p) + "_" + _crtPos(p) + "_" + _crtPos(c));
	// p-pos,c-word,c-pos
	featVec.push_back(_crtPos(p) + "_" + _crtWord(c) + "_" + _crtPos(c));
	// p-word,p-pos,c-word,c-pos
	featVec.push_back(_crtWord(p) + "_" + _crtPos(p) + "_" + _crtWord(c) + "_" + _crtPos(c));
	// p-pos
	featVec.push_back(_crtPos(p));
	// c-pos
	featVec.push_back(_crtPos(c));
	// p-pos,c-pos
	featVec.push_back(_crtPos(p) + "_" + _crtPos(c));
	// p-pos,p-pos+1,c-pos-1,c-pos
	featVec.push_back(_crtPos(p) + "_" + _nextPos(p) + "_" + _prePos(c) + "_" + _crtPos(c));
	// p-pos-1,p-pos,c-pos-1,c-pos
	featVec.push_back(_prePos(p) + "_" + _crtPos(p) + "_" + _prePos(c) + "_" + _crtPos(c));
	// p-pos,p-pos+1,c-pos,c-pos+1
	featVec.push_back(_crtPos(p) + "_" + _nextPos(p) + "_" + _crtPos(c) + "_" + _nextPos(c));
	// p-pos-1,p-pos,c-pos,c-pos+1
	featVec.push_back(_prePos(p) + "_" + _crtPos(p) + "_" + _crtPos(c) + "_" + _nextPos(c));
	return true;
}

bool FeatureTemplate::_initHeader(string & str, int pos)
{
	if(pos == p) str = "P:";
	else str = "C:";
	return true;
}

string FeatureTemplate::_crtWord(int pos)
{
	string res;
	_initHeader(res, pos);
	res += "CW:";
	return res + (*pSen)[pos].first;
}

string FeatureTemplate::_crtPos(int pos)
{
	string res;
	_initHeader(res, pos);
	res += "CP:";
	return res + (*pSen)[pos].second;
}

string FeatureTemplate::_prePos(int pos)
{
	string res;
	_initHeader(res, pos);
	res += "PP:";
	pos--;
	if(pos < 0) res += " ";
	else res += (*pSen)[pos].second;
	return res;
}

string FeatureTemplate::_nextPos(int pos)
{
	string res;
	_initHeader(res, pos);
	res += "NP:";
	pos++;
	if(pos >= (int)(*pSen).size()) res += " ";
	else res += (*pSen)[pos].second;
	return res;
}
