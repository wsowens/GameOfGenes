#include "Util.h"
using namespace std;

int fs_atoi(ifstream &input)
{
	string l;
	getline(input, l);
	return atoi(l.c_str());
}

bool endsWith(string s, string ending)
{
	return (s.rfind(ending) + ending.length()) == s.length();
}
