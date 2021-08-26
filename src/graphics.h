#pragma once

#include "Display.h"

#define DETECT 0

Display* d = nullptr;

void initwindow()
{
	d = new Display();
}

void initgraph(int* argc, int* argcc, Renderer* r)
{
	d->initPanel(r);		// initialize control panel using imgui
	glb.clock.init();		// fps, rotation, camera movement
}

bool isRunning() {
	return d->isRunning();
}

void cleardevice() {
	d->clear();
}

void updatedevice() {
	d->update();
}

void closegraph() {
	delete d;
}
