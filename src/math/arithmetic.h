#pragma once

#include "define.h"

namespace math
{
	// Returns maximum value among values: v1 & v2
	template <typename T>
	T max(T v1, T v2) {
		return	v1 > v2 ? v1 : v2;
	}

	// Swaps 'pointer to data types'
	template <typename T>
	void pswap(T** p1, T** p2)
	{
		T* pTemp = *p1;
		*p1 = *p2;
		*p2 = pTemp;
	}

	// Returns absolute value aka modulus
	template <typename T>
	inline T abs(T&& ival)
	{
		return ival < 0 ? -ival : ival;
	}

	// Convert degree to radians
	inline float radians(float d) {
		return	(d * PI / 180);
	}
	// Convert radians to degree
	inline float degree(float r) {
		return	(r * 180 / PI);
	}

	// Linear Interpolation aka Lerp
	// Interpolate values from d0 to d1 through i0 to i1 -> i& d are independent and dependent variables : d = f(i)
	template <typename T>
	inline void interpolate(std::vector<T>& values, int i0, T d0, int i1, T d1)
	{
		if (i0 == i1)
		{
			values.emplace_back((T)d0);
			return;
		}
		float a = (float)(d1 - d0) / (i1 - i0);	// increment value
		float d = d0;
		for (int i = i0; i <= i1; i++)
		{
			values.emplace_back((T)d);
			d = d + a;
		}
		return;
	}

	// Clamp value between max and min : limits to nearest available value
	template <typename T>
	inline void clamp(T& var, T min, T max) {
		var = var<min ? min : var>max ? max : var;
	}
};