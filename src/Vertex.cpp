#pragma once

#include "Vertex.h"

void Vertex::SWAP(Vertex& v1, Vertex& v2) {
	Vertex temp = v1;
	v1 = v2;
	v2 = temp;
}