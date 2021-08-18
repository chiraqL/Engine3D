#include "main.h"

int main(int* argc, char* argv)
{
	initwindow();	//initialize window
	Renderer r;

	// Initialize graphics
	int gd = DETECT, gm;
	initgraph(&gd, &gm, &r);

	while (isRunning())
	{
		cleardevice();		// clear display buffers
		r.update();			// update buffers
		updatedevice();		// update gpu buffer
	}
	closegraph();

	CONSOLE_PAUSE();
	return 0;
}