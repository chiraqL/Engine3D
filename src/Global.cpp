#pragma once

#include "Global.h"

Global glb;

void View::reset()
{
	r = false;
	l = false;
	t = false;
	f = false;
}

bool View::isDefault()
{
	return !(r || l || t || f);
}

void Shading::setShading(bool type)
{
	switch (type)
	{
	case FLAT:
		type = FLAT;	break;
	case GOURAUD:
		type = GOURAUD; break;
	default:
		break;
	};
}