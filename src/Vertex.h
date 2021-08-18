#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"
#include "Color.h"

struct Vertex
{
	vec4 position;
	vec3 normal;
	tex2 tex;
	static void SWAP(Vertex& v1, Vertex& v2);
};

typedef struct Vertex vert;

#endif