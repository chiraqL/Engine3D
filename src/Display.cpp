#include "Display.h"

void Display::init(const char* title)
{
	running = true;

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);	// 4.6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);		// Disable window resize
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);		// and titlebar

	// Create window
	window = glfwCreateWindow(glb.draw.win_width, glb.draw.win_height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowPos(window, 100, 50);	// window starting position offset
	glfwMakeContextCurrent(window);

	// Initialize OpenGL, z & color buffers, Map gl buffer
	glb.draw.init();
}

// Initialize ImGUI panel
void Display::initPanel(Renderer* rr)
{
	r = rr;
	panel = new Panel(window, rr);
}

// Input handling, Clear and start panel, Clear buffers
void Display::clear()
{
	// Handle keyboard inputs
	processInput();
	running = !glfwWindowShouldClose(window);
	// Calculate Fps, setup panel render items
	panel->pre_render();
	// Clear buffers
	glb.draw.clear();
}

// Draw to window, Swap window buffers
void Display::update()
{
	glb.draw.update();
	panel->post_render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// Close window
void Display::close()
{
	delete panel;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Display::framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	glb.draw.win_width = w + glb.draw.pan_width;
	glb.draw.win_height = h;
	glb.draw.pan_height = h;
}

// Handle inputs
void Display::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		r->getCamera()->eye = r->getCamera()->eye + r->getCamera()->forwardDir * 0.1f;
		glb.zio = math::length(r->getCamera()->target - r->getCamera()->eye);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		r->getCamera()->eye = r->getCamera()->eye - r->getCamera()->forwardDir * 0.1f;
		glb.zio = math::length(r->getCamera()->target - r->getCamera()->eye);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		glb.camYaw += glb.tElapsed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		glb.camYaw -= glb.tElapsed;
}
