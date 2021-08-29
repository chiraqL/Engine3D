#pragma once

#include <limits>
#include <cassert>
#include <algorithm>

#include "Color.h"

class PBuffer
{
public:
	PBuffer(int width, int height)
		:
		width(width),
		height(height),
		pBuffer(new col3[width * height]),
		tBuffer(new bool[width * height])
	{}
	~PBuffer()
	{
		delete[] pBuffer;
		delete[] tBuffer;
		pBuffer = nullptr;
		tBuffer = nullptr;
	}
	// Disallow copying
	PBuffer(const PBuffer&) = delete;
	PBuffer& operator=(const PBuffer&) = delete;
	void Clear(col3& color)
	{
		const int nPixels = width * height;
		for (int i = 0; i < nPixels; i++)
		{
			pBuffer[i] = color;//{ 0.0f,0.0f,0.0f }
			tBuffer[i] = false;
		}
	}
	col3& At(int x, int y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return pBuffer[y * width + x];
	}
	bool& tAt(int x, int y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return tBuffer[y * width + x];
	}
	const col3& At(int x, int y) const
	{
		return const_cast<PBuffer*>(this)->At(x, y);
	}
	void set(int x, int y, col3 color)
	{
		col3& colorInBuffer = At(x, y);
		bool& isColorChanged = tAt(x, y);
		colorInBuffer = color;
		isColorChanged = true;
	}
private:
	int width;
	int height;
	col3* pBuffer = nullptr;	// pointer to color buffer
	bool* tBuffer = nullptr;
};