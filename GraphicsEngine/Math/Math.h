#pragma once

#ifndef GRAPHICSENGINE__MATH_H
#define GRAPHICSENGINE__MATH_H


#include <cmath>


namespace GraphicsEngine
{
	/**
	Aggregates useful math functions
	**/

	const float PI = 3.141592654f;

	__forceinline float DegreesToRadians(float degs)
	{
		return degs * (PI / 180.0f);
	}

	__forceinline float RadiansToDegrees(float rads)
	{
		return rads * (180.0f / PI);
	}

	template<typename T>
	__forceinline T Min(const T& a, const T& b)
	{
		return (a < b ? a : b);
	}

	template<typename T>
	__forceinline T Max(const T& a, const T& b)
	{
		return (a > b ? a : b);
	}

	template<typename T>
	__forceinline T Lerp(const T& lo, const T& hi, const T& t)
	{
		return (1-t)*lo + t*hi;
	}

	template<typename T>
	__forceinline T UniformRandom()
	{
		return ((T)rand())/RAND_MAX;
	}

	template<typename T>
	__forceinline T UniformRandom(const T& lo, const T& hi)
	{
		T unitrand = UniformRandom<T>();
		return Lerp(lo, hi, unitrand);
	}

	__forceinline int RoundUpPow2(int num)
	{
		int p = 1;
		while (p < num)
			p *= 2;
		return p;
	}
}


#endif