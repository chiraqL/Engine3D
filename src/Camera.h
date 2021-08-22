#pragma once

#include "Vector.h"

class Camera
{
public:
	vec3 eye;		// position in World
	vec3 up;		// up direction
	vec3 target;	// point to
	vec3 lookDir;	// look at direction
	vec3 forwardDir;// forward vector
public:
	Camera()
		:
		eye(ORIGIN),
		up(UP),
		target(FORWARD),
		lookDir(FORWARD),
		forwardDir(FORWARD)
	{};
	Camera (vec3& pos, vec3& up, vec3& target)
		:
		eye(pos),
		up(up),
		target(target),
		lookDir(target),
		forwardDir(target)
	{};
	~Camera() {};
};