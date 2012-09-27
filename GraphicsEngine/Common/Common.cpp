#include "Common/Common.h"

using namespace std;

vector<string> SplitString(const string& str, const string& delim)
{
	UINT i = 0;
	UINT p;
	vector<string> result;

	do
	{
		p = str.find(delim, i);
		if (p > i)
			result.push_back(str.substr(i, p-i));
		i = p+1;
	}
	while (p != string::npos);

	return result;
}

string LStrip(const string& str, char delim)
{
	string result = str;
	while (result.front() == delim)
	{
		if (result.length() == 1)
			result = "";
		else
			result = result.substr(1);
	}
	return result;
}

string RStrip(const string& str, char delim)
{
	string result = str;
	while (result.back() == delim)
	{
		if (result.length() == 1)
			result = "";
		else
			result.pop_back();
	}
	return result;
}

string LRStrip(const string& str, char delim)
{
	return LStrip(RStrip(str, delim), delim);
}

string FindAndReplace(const string& searchMe, const string& replaceThis, const string& withThis)
{
	string result = searchMe;
	for (UINT t = result.find(replaceThis); t != result.npos; t = result.find(replaceThis))
	{
		result.replace(t, replaceThis.length(), withThis);
	}
	return result;
}