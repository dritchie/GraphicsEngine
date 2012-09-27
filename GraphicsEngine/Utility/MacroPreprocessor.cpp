#include "Utility/MacroPreprocessor.h"
#include "Utility/FileSystem.h"
#include "Math/Math.h"
#include <set>
#include <map>
#include <unordered_map>
#include <sstream>

using namespace std;

namespace GraphicsEngine
{
	MacroPreprocessor::MacroPreprocessor(const string& shaderPath)
	{
		paths = SplitString(shaderPath, ";");

		// The empty string is always a valid path
		paths.push_back("");
	}

	string MacroPreprocessor::ResolvePath(const string& filename)
	{
		// Try to find the file in the path
		string resolvedname;
		bool foundit = false;
		for (UINT i = 0; i < paths.size(); i++)
		{
			resolvedname = paths[i] + filename;
			if (FileExists(resolvedname))
			{
				foundit = true;
				break;
			}
		}
		if (!foundit)
			FatalError(string("MacroPreprocessor - Could not find file '" + filename + "'"))
		return resolvedname;
	}

	void MacroPreprocessor::InitFromSource(const string& source)
	{
		lines = SplitString(source, "\n");
	}

	void MacroPreprocessor::InitFromFile(const string& filename)
	{
		lines = FileLines(ResolvePath(filename));
	}

	string MacroPreprocessor::GetParseResults()
	{
		string text = "";
		for (UINT i = 0; i < lines.size(); i++)
			text += (lines[i] + "\n");
		return text;
	}

	void MacroPreprocessor::CompleteParse()
	{
		IncludePass();
		BlockPass();
		DefinePass();
	}

	string RestOfLine(stringstream& line)
	{
		char restofline[1024];
		line.get(restofline, 1023);
		string str(restofline);
		return LRStrip(LRStrip(str, ' '), '\t');
	}

	void MacroPreprocessor::IncludePass()
	{
		UINT i = 0;
		while (i < lines.size())
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#include")
			{
				string incfile = RestOfLine(ss);

				// Strip the leading and trailing " from the filename
				incfile = incfile.substr(1, incfile.length()-2);

				// Find it in the path
				string realincfile = ResolvePath(incfile);

				// Get lines from the file, add them to be parsed
				// (Don't forget to delete the current line)
				vector<string> inclines = FileLines(realincfile);
				lines.erase(lines.begin()+i);
				lines.insert(lines.begin()+i, inclines.begin(), inclines.end());
			}
			else i++;
		}
	}

	string MacroPreprocessor::NearbyLinesString(int linenum)
	{
		const int support = 2;
			int mini = Max(0, linenum - support);
			int size = (int)(this->lines.size());
			int maxi = Min(size, linenum + support);
			string result = "";
			for (int j = mini; j < maxi; j++)
				result += (this->lines[j] + "\n");
			return result;
	}

	void MacroPreprocessor::BlockPass()
	{
		set<UINT> beginLines, endLines;
		map<UINT, UINT> blockMap;
		std::unordered_map<std::string, std::string> blocks;

		auto UpdateLineNumbers = [&blockMap] (UINT startingHere, UINT removedThisMany) -> void
		{
			map<UINT, UINT> newBlockMap;
			for (auto it = blockMap.begin(); it != blockMap.end(); it++)
			{
				UINT defLine = it->first;
				UINT enddefLine = it->second;
				if (defLine > startingHere)
					defLine -= removedThisMany;
				if (enddefLine > startingHere)
					enddefLine -= removedThisMany;
				newBlockMap[defLine] = enddefLine;
			}
			blockMap = newBlockMap;
		};

		// First, make a pass over all lines and record positions
		// of all the # directives
		for (UINT i = 0; i < lines.size(); i++)
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#blockbegin")
				beginLines.insert(i);
			else if (op == "#blockend")
				endLines.insert(i);
		}

		// Match up corresponding begin/end lines
		for (auto it = beginLines.rbegin(); it != beginLines.rend(); it++)
		{
			UINT defLine = *it;
			auto endit = endLines.upper_bound(defLine);
			if (endit == endLines.end())
				FatalError(string("MacroPreprocessor - unmatched #blockbegin around \n\n") + NearbyLinesString(defLine))
			UINT endLine = *endit;
			blockMap[defLine] = endLine;
			endLines.erase(endit);
		}
		// Any remaining #enddefs are mysterious; we die
		for (auto it = endLines.begin(); it != endLines.end(); it++)
			FatalError(string("MacroPreprocessor - unmatched #blockend around \n\n") + NearbyLinesString(*it))

		// Process all of the blocks, record them in a map
		UINT i = 0;
		while (i < lines.size())
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#blockbegin")
			{
				string blockkey; ss >> blockkey;

				// Concatenate all lines between this and the coresponding #blockend;
				// this is the defined value
				UINT endline = blockMap[i];
				string blockval = "";
				for (UINT j = i+1; j < endline; j++)
					blockval += (lines[j] + "\n");
				blocks[blockkey] = blockval;

				// Move to the line after the #blockend line
				lines.erase(lines.begin() + i, lines.begin() + endline + 1);
				UpdateLineNumbers(i, endline - i + 1);
			}
			else
			{
				i++;
			}
		}

		// Now make another pass to expand all the #block directives
		i = 0;
		while (i < lines.size())
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#block")
			{
				string blockkey; ss >> blockkey;
				auto it = blocks.find(blockkey);
				if (it == blocks.end())
					FatalError(string("MacroPreprocessor - Use of undefined #block near\n\n") + NearbyLinesString(i));
				string blockval = it->second;

				// Split the block on newlines, insert the result into 'lines'
				vector<string> blocklines = SplitString(blockval, "\n");
				lines.erase(lines.begin()+i);
				lines.insert(lines.begin()+i, blocklines.begin(), blocklines.end());
				UpdateLineNumbers(i, -((int)(blocklines.size()-1)));
			}
			else
			{
				i++;
			}
		}
	}


	struct IfdefBlock
	{
		UINT ifdefLine;
		UINT elseLine;
		UINT endifLine;
	};

	void MacroPreprocessor::DefinePass()
	{
		unordered_map<string, string> defines;
		set<UINT> ifdefLines, elseLines, endifLines;
		map<UINT, IfdefBlock> ifdefBlockMap;

		auto ReplaceDefines = [&defines] (string& str) -> void
		{
			for (auto it = defines.begin(); it != defines.end(); it++)
				str = FindAndReplace(str, it->first, it->second);
		};

		auto UpdateLineNumbers = [&ifdefBlockMap] (UINT startingHere, int removedThisMany) -> void
		{
			// Update #ifdef blocks
			map<UINT, IfdefBlock> newIfdefBlockMap;
			for (auto it = ifdefBlockMap.begin(); it != ifdefBlockMap.end(); it++)
			{
				IfdefBlock block = it->second;
				if (block.ifdefLine > startingHere)
					block.ifdefLine -= removedThisMany;
				if (block.elseLine > startingHere)
					block.elseLine -= removedThisMany;
				if (block.endifLine > startingHere)
					block.endifLine -= removedThisMany;
				newIfdefBlockMap[block.ifdefLine] = block;
			}
			ifdefBlockMap = newIfdefBlockMap;
		};


		// First, make a pass over all lines and record positions
		// of all the # directives
		for (UINT i = 0; i < lines.size(); i++)
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#ifdef" || op == "#ifndef")
				ifdefLines.insert(i);
			else if (op == "#else")
				elseLines.insert(i);
			else if (op == "#endif")
				endifLines.insert(i);
		}

		// Next, make a second pass, associating corresponding #ifdefs, #elses,
		// and #endifs into IfdefBlocks.
		for (auto it = ifdefLines.rbegin(); it != ifdefLines.rend(); it++)
		{
			UINT ifdefLine = *it;
			IfdefBlock& block = ifdefBlockMap[ifdefLine];
			block.ifdefLine = ifdefLine;

			auto endifit = endifLines.upper_bound(ifdefLine);
			if (endifit == endifLines.end())
				FatalError(string("MacroPreprocessor - unmatched #ifdef around \n\n" + NearbyLinesString(ifdefLine)))
			block.endifLine = *endifit;
			endifLines.erase(endifit);
			auto elseit = elseLines.upper_bound(ifdefLine);
			block.elseLine = block.endifLine;
			if (elseit != elseLines.end() && *elseit < block.endifLine)
			{
				block.elseLine = *elseit;
				elseLines.erase(elseit);
			}
		}
		// Any remaining elses or endifs are mysterious and unaccounted for, so we should die.
		for (auto it = elseLines.begin(); it != elseLines.end(); it++)
			FatalError(string("MacroPreprocessor - unmatched #else around \n\n" + NearbyLinesString(*it)))
		for (auto it = endifLines.begin(); it != endifLines.end(); it++)
			FatalError(string("MacroPreprocessor - unmatched #endif around \n\n" + NearbyLinesString(*it)))

		// Now make the final pass: instantiate all #define directives and
		// correctly deal with all #ifdef blocks, and fill in all #block directives
		UINT i = 0;
		while (i < lines.size())
		{
			stringstream ss; ss.str(lines[i]);
			string op; ss >> op;
			if (op == "#define")
			{
				string defkey; ss >> defkey;
				string defval = RestOfLine(ss);
				ReplaceDefines(defval);
				defines[defkey] = defval;

				// Delete this line and move on to the next one
				lines.erase(lines.begin() + i);
				UpdateLineNumbers(i, 1);
			}
			else if (op == "#ifdef" || op == "#ifndef")
			{
				//cout << "BEFORE --------------------------------------" << endl << GetParseResults() << endl;

				string defkey; ss >> defkey;
				auto it = ifdefBlockMap.find(i);
				IfdefBlock block = it->second;
				ifdefBlockMap.erase(it);

				// If we satisfy the condition, remove everything between the else
				// and the endif
				// Also remove the ifdef
				if (op == "#ifdef" && defines.count(defkey) ||
					op == "#ifndef" && !defines.count(defkey))
				{
					lines.erase(lines.begin()+block.elseLine, lines.begin()+block.endifLine+1);
					UpdateLineNumbers(block.elseLine, block.endifLine - block.elseLine + 1);
					lines.erase(lines.begin() + i);
					UpdateLineNumbers(i, 1);
				}
				// Otherwise, remove everything between the ifdef and the else
				// Also remove the endif
				else
				{
					if (block.elseLine != block.endifLine)
					{
						lines.erase(lines.begin() + block.endifLine);
						UpdateLineNumbers(block.endifLine, 1);
					}
					lines.erase(lines.begin()+block.ifdefLine, lines.begin()+block.elseLine+1);
					UpdateLineNumbers(block.ifdefLine, block.elseLine - block.ifdefLine + 1);
				}

				//cout << "AFTER -----------------------------------------" << endl << GetParseResults() << endl;
			}
			else
			{
				ReplaceDefines(lines[i]);
				i++;
			}
		}
	}
}