#pragma once

#ifndef GRAPHICSENGINE__GRAPHICS_APP_PARAMS_H
#define GRAPHICSENGINE__GRAPHICS_APP_PARAMS_H


#include "Common/Common.h"
#include <unordered_map>


namespace GraphicsEngine
{
	/**
	Loads and makes available single-line parameters from a text file
	Supports string, bool, int, and float params
	**/
	class GraphicsAppParams
	{
	public:
		void Init(const std::string& paramfile);

		// Read param values
		bool BoolParam(const std::string& name) const;
		int IntParam(const std::string& name) const;
		double FloatParam(const std::string& name) const;
		const std::string& StringParam(const std::string& name) const;

		// Set param values
		bool& BoolParam(const std::string& name);
		int& IntParam(const std::string& name);
		double& FloatParam(const std::string& name);
		std::string& StringParam(const std::string& name);

	private:
		std::unordered_map<std::string, bool> paramsBool;
		std::unordered_map<std::string, int> paramsInt;
		std::unordered_map<std::string, double> paramsFloat;
		std::unordered_map<std::string, std::string> paramsString;
	};
}


#endif