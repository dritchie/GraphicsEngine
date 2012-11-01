#include "Utility/FileSystem.h"
#include <fstream>

using namespace std;

namespace GraphicsEngine
{

	string AbsolutePath(const string& relPath)
	{
#ifdef WIN32
		const char* fileName = relPath.c_str();
		const DWORD bufLen = MAX_PATH + 1;
		char outFileName[bufLen];
		GetFullPathName(fileName, bufLen, outFileName, NULL);
		return string(outFileName);
#else
		const char* fileName = relPath.c_str();
		char* outFileName[PATH_MAX + 1];
		realpath(fileName, outFileName);
		return string(outFileName);
#endif
	}

	bool FileExists(const string& filename)
	{
		ifstream infile(filename.c_str());
		if (infile)
		{
			infile.close();
			return true;
		}
		else
		{
			infile.close();
			return false;
		}
	}

	string TextFromFile(const string& filename)
	{
		ifstream infile(filename.c_str());
		if (!infile)
			FatalError(string("GraphicsEngine::TextFromFile - No such file ' " + filename + "'"))

		char line[1024];
		string result = "";
		while (!infile.eof())
		{
			infile.getline(line, 1023);
			result += (string(line) + "\n");
		}
		infile.close();
		
		return result;
	}

	vector<string> FileLines(const string& filename)
	{
		ifstream infile(filename.c_str());
		if (!infile)
			FatalError(string("GraphicsEngine::TextFromFile - No such file ' " + filename + "'"))

		char line[1024];
		vector<string> result;
		while (!infile.eof())
		{
			infile.getline(line, 1023);
			string strline = string(line);
			if (!strline.empty())
				result.push_back(strline);
		}
		infile.close();
		
		return result;
	}

	void SaveTextToFile(const string& text, const string& filename)
	{
		ofstream outfile(filename.c_str());
		if (!outfile)
			FatalError(string("GraphicsEngine::SaveTextToFile - Couldn't write to file '") + filename + "'")
		outfile << text;
		outfile.close();
	}

}