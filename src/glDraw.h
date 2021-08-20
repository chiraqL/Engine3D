#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>

#include "ZBuffer.h"
#include "PBuffer.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();x;ASSERT(GLLogError())

struct ShaderSource
{
	std::string vs;
	std::string fs;
};
struct Buffer
{
	float x, y, r, g, b;
};
struct Range	// for buffering: not required
{
	size_t begin = 0;
	GLsync sync = 0;
};
class glDraw
{
public:
	Range gSyncRanges[2];// for buffering: not required
	GLuint gRangeIndex = 0;// for buffering: not required
	GLuint gWaitCount = 0;// for buffering: not required
	ZBuffer* zbuffer;
	PBuffer* pbuffer;
	col3 bgCol = { 1.0f, 1.0f, 1.0f };
	col3 invBgCol = { 1.0f, 1.0f, 1.0f };	//inverted bg color
	unsigned long x = 1, y = 5, z = 9;
	unsigned int win_width = 1200;	//window
	unsigned int win_height = 900;
	unsigned int pan_width = 300;	//panel
	unsigned int pan_height = 900;
	unsigned int vp_width = 900;	//viewport
	unsigned int vp_height = 900;
	glDraw() {}
	~glDraw() {}
	float* pMapBuffer;
private:
	unsigned long long pixelSize = 0;
	unsigned int vao, vbo, program;
	GLsync gSync;
public:
	void init(void);
	void initShader();
	void resize(unsigned int ww, unsigned int wh, unsigned int pw, unsigned int ph);
	void clear();
	void GLClearErrors();
	bool GLLogError();
	struct ShaderSource parseShader(const std::string& path);
	unsigned int compileShader(unsigned int type, const std::string& src);
	unsigned int createShader(const std::string& vs, const std::string& fs);
	void lockBuffer();
	void waitBuffer();
	void putpixel(int x, int y, float z, col3& col);
	void update();
	void close(void);
};
typedef glDraw gld;

