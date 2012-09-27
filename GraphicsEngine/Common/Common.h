#pragma once

#ifndef GRAPHICSENGINE__COMMON_H
#define GRAPHICSENGINE__COMMON_H

/**
Common includes, macros, functions, and typedefs that are used
across the board
**/


/*************************************** INCLUDES *******************************************/

#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#ifdef WIN32
#include <windows.h>
#endif


/*************************************** MACROS ********************************************/

// Make __forceinline available in gcc, as well
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif


// Safely delete pointers
#define SAFEDELETE(p) if (p) delete (p);

// Cross-platform safe printf
#ifdef WIN32
#define SafePrintf sprintf_s
#else
#define SafePrintf snprintf
#endif

// Used for throwing fatal errors
#ifdef WIN32
#define FatalError(msg) { MessageBox(NULL, (msg).c_str(), "GraphicsEngine - FatalError", MB_OK); throw 0; }
//#define FatalError(msg) { std::cerr << "GraphicsEngine - FatalError: " << (msg).c_str() << std::endl; throw 0; }
#else
#define FatalError(msg) { std::cerr << "GraphicsEngine - FatalError: " << (msg).c_str() << std::endl; throw 0; }
#endif


// Non-fatal warnings
#ifdef WIN32
#define Warning(msg) { MessageBox(NULL, (msg).c_str(), "GraphicsEngine - Warning", MB_OK); }
#else
#define Warning(msg) { std::cerr << "GraphicsEngine - Warning: " << (msg).c_str() << std::endl; }
#endif


/*************************************** TYPEDEFS ******************************************/

typedef unsigned int UINT;


/*************************************** FUNCTIONS *****************************************/

template<typename T>
void FreeVectorMemory(std::vector<T>& toClear)
{
	std::vector<T>().swap(toClear);
}

template<typename T>
void VectorArrayCopy(std::vector<T>& dst, T* src, UINT n)
{
	dst.resize(n);
	for (UINT i = 0; i < n; i++)
		dst[i] = src[i];
}

template<typename T>
bool VectorContains(const std::vector<T>& vec, const T& val)
{
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (*it == val)
			return true;
	}
	return false;
}

std::vector<std::string> SplitString(const std::string& str, const std::string& delim);

std::string LStrip(const std::string& str, char delim);
std::string RStrip(const std::string& str, char delim);
std::string LRStrip(const std::string& str, char delim);

std::string FindAndReplace(const std::string& searchMe, const std::string& replaceThis, const std::string& withThis);


#endif