#pragma once

#ifndef GRAPHICS_ENGINE__MACRO_PREPROCESSOR_H
#define GRAPHICS_ENGINE__MACRO_PREPROCESSOR_H

#include "Common/Common.h"
#include <unordered_map>

namespace GraphicsEngine
{
	/**
	Parses text files, dealing with #include, #define, #ifdef,
	#ifndef, #else, and #endif directives
	**/
	class MacroPreprocessor
	{
	public:

		MacroPreprocessor(const std::string& includePath);

		void InitFromSource(const std::string& source);
		void InitFromFile(const std::string& filename);

		void CompleteParse();

		std::string GetParseResults();

	private:

		std::string ResolvePath(const std::string& filename);
		std::string NearbyLinesString(int linenum);

		void IncludePass();
		void BlockPass();
		void DefinePass();

		std::vector<std::string> lines;
		std::vector<std::string> paths;
	};
}


#endif