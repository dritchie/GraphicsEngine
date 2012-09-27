#include "Assets/ShaderAsset.h"

using namespace std;

namespace GraphicsEngine
{
	string ShaderAsset::Define(const string& key, const string& value)
	{
		return "#define " + key + " " + value + "\n";
	}

	string ShaderAsset::Block(const string& key, const vector<string>& lines)
	{
		string result = "#blockbegin " + key + "\n";
		for (UINT i = 0; i < lines.size(); i++)
		{
			result += lines[i] + "\n";
		}
		result += "#blockend\n";
		return result;
	}

	string ShaderAsset::Block(const string& key, const string& contents)
	{
		return "#blockbegin " + key + "\n" + contents + "\n#blockend\n";
	}
}