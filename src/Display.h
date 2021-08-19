#pragma once

#include <algorithm>

#include "Timer.h"
#include "Panel.h"
#include "Global.h"

class Display
{
private:
	GLFWwindow* window = nullptr;
	Panel* panel = nullptr;
	bool running;
	char** pixels = nullptr;
	Renderer* r;
public:
	Display() { init("3D Cottage"); }
	~Display() { close(); }

	bool isRunning() { return running; }
	void init(const char* title);
	void initPanel(Renderer* r);
	void clear();
	void update();
	void close();
	void processInput();

	// Window resize callback
	static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
};

