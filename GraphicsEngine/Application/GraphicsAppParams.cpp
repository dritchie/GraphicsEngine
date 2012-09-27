#include "Application/GraphicsAppParams.h"
#include <fstream>
#include <sstream>

using namespace std;

namespace GraphicsEngine
{
	void GraphicsAppParams::Init(const string& paramfile)
	{
		// Parse params from file
		std::ifstream infile(paramfile.c_str());
		if (!infile)
			FatalError(string("GraphicsAppParams::Init - Couldn't open parameters file '" + paramfile + "'"))

		char line[1024];
		while (!infile.eof())
		{
			infile.getline(line, 1023);
			std::stringstream ss;
			ss.str(line);

			string pName, pValue;
			ss >> pName >> pValue;

			// CASE: Empty lines and comments
			if (pName.length() == 0 || pName[0] == '#') continue;

			// CASE: Boolean parameter
			if (pValue == "true" || pValue == "false")
			{
				paramsBool[pName] = (pValue == "true" ? true : false);
				continue;
			}
			
			// CASE: Numerical parameters
			double num;
			std::stringstream ss2;
			ss2.str(pValue);
			if (ss2 >> num)
			{
				// Is it an integer or a float type?
				if (pValue.find('.') != pValue.npos)
					paramsFloat[pName] = num;
				else
					paramsInt[pName] = (int)num;
				continue;
			}

			// CASE: string parameters (i.e. everything else)
			paramsString[pName] = pValue;
		}
		infile.close();
	}

	// Read param values
	bool GraphicsAppParams::BoolParam(const string& name) const
	{
		auto it = paramsBool.find(name);
		if (it != paramsBool.end())
			return it->second;
		else
			FatalError("GraphicsAppParams::BoolParam - No parameter named '" + name + "'");
	}
	int GraphicsAppParams::IntParam(const string& name) const
	{
		auto it = paramsInt.find(name);
		if (it != paramsInt.end())
			return it->second;
		else
			FatalError("GraphicsAppParams::IntParam - No parameter named '" + name + "'");
	}
	double GraphicsAppParams::FloatParam(const string& name) const
	{
		auto it = paramsFloat.find(name);
		if (it != paramsFloat.end())
			return it->second;
		else
			FatalError("GraphicsAppParams::FloatParam - No parameter named '" + name + "'");
	}
	const string& GraphicsAppParams::StringParam(const string& name) const
	{
		auto it = paramsString.find(name);
		if (it != paramsString.end())
			return it->second;
		else
			FatalError("GraphicsAppParams::stringParam - No parameter named '" + name + "'");
	}

	// Set param values
	bool& GraphicsAppParams::BoolParam(const string& name)
	{
		auto it = paramsBool.find(name);
		if (it != paramsBool.end())
			return paramsBool[name];
		else
			FatalError("GraphicsAppParams::BoolParam - No parameter named '" + name + "'");
	}
	int& GraphicsAppParams::IntParam(const string& name)
	{
		auto it = paramsInt.find(name);
		if (it != paramsInt.end())
			return paramsInt[name];
		else
			FatalError("GraphicsAppParams::IntParam - No parameter named '" + name + "'");
	}
	double& GraphicsAppParams::FloatParam(const string& name)
	{
		auto it = paramsFloat.find(name);
		if (it != paramsFloat.end())
			return paramsFloat[name];
		else
			FatalError("GraphicsAppParams::FloatParam - No parameter named '" + name + "'");
	}
	string& GraphicsAppParams::StringParam(const string& name)
	{
		auto it = paramsString.find(name);
		if (it != paramsString.end())
			return paramsString[name];
		else
			FatalError("GraphicsAppParams::stringParam - No parameter named '" + name + "'");
	}
}