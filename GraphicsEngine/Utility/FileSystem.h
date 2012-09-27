#pragma once

#ifndef GRAPHICS_ENGINE__FILE_SYSTEM_H
#define GRAPHICS_ENGINE__FILE_SYSTEM_H

#include "Common/Common.h"

namespace GraphicsEngine
{
	std::string AbsolutePath(const std::string& relPath);

	bool FileExists(const std::string& filename);

	std::string TextFromFile(const std::string& filename);

	std::vector<std::string> FileLines(const std::string& filename);

	void SaveTextToFile(const std::string& text, const std::string& filename);
}


#endif