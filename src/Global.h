#pragma once

#include "Clock.h"
#include "Controls.h"
#include "glDraw.h"

struct View
{
	bool r, l, t, f;	// right left top front view of model
	void reset();		// reset to default view
	bool isDefault();	// whether default view or not
public:
	View()				// initialize to default view
		:
		r(false),
		l(false),
		t(false),
		f(false)
	{};
};

enum ShadingType { FLAT, GOURAUD };

struct Shading
{
	int type;					// shading algorithm used: flat or gouraud
	void setShading(bool type);	// set/change shading type
public:
	Shading()					// initialize with Gouraud shading
		:
		type(GOURAUD)
	{};
};

class Global
{
public:
	Shading shading;		// flat/gouraud
	View view;				// right/left/top/front view
	Clock clock;
	Controls controls;		// user controls
	glDraw draw;
	unsigned framecount;
	double tDelta;
	float fps, camYaw, zio, tElapsed;
public:
	Global()
		:
		framecount(0),
		tDelta(0),
		fps(0),
		camYaw(0.0f),
		zio(1.0f),
		tElapsed(0.0f)
	{};
};

extern Global glb;