#include "Util.h"
using namespace std;

int fs_atoi(ifstream &input)
{
	string l;
	getline(input, l);
	if(input.eof())
		throw "EOF Reached";
	return atoi(l.c_str());
}

bool endsWith(string s, string ending)
{
	return (s.rfind(ending) + ending.length()) == s.length();
}

string separator()
{
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
    return "\\";
#else
    return "/";
#endif
}
