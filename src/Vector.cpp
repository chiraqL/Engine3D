#pragma once

#include "Vector.h"

std::ostream& operator<<(std::ostream& os, const Vector3d& vec)
{
	os << "( " << vec.x << " , " << vec.y << " , " << vec.z << " )";
	return os;
}