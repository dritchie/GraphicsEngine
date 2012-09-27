#include "Assets/ShaderAsset.h"

using namespace std;

namespace GraphicsEngine
{

	void ShaderAssetCode::AddVertCode(const std::string& code)
	{
		if (!VectorContains(vertCode, code))
			vertCode.push_back(code);
	}

	void ShaderAssetCode::AddGeoCode(const std::string& code)
	{
		if (!VectorContains(geoCode, code))
			geoCode.push_back(code);
	}

	void ShaderAssetCode::AddTessControlCode(const std::string& code)
	{
		if (!VectorContains(tessControlCode, code))
			tessControlCode.push_back(code);
	}

	void ShaderAssetCode::AddTessEvalCode(const std::string& code)
	{
		if (!VectorContains(tessEvalCode, code))
			tessEvalCode.push_back(code);
	}

	void ShaderAssetCode::AddFragCode(const std::string& code)
	{
		if (!VectorContains(fragCode, code))
			fragCode.push_back(code);
	}




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