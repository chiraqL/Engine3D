#pragma once

#include "define.h"
#include "../Matrix.h"
#include "../Camera.h"

namespace math
{
	// Swap Vertex data
	inline void SWAP(vert& v1, vert& v2)
	{
		vert t1 = v1;
		v1 = v2;
		v2 = t1;
	}

	// Create Identity matrix -> I
	inline mat4 identity() {
		return	{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			0,	0,	0,	1
		};
	}

	// Create Orthographic matrix
	inline mat4 orthogonal(float l = -1, float r = 1, float t = 1, float b = -1, float n = 0.1, float f = 100) {
		return	{
			2 / (r - l),				0,						0,								0,
			0,							2 / (t - b),			0,								0,
			0,							0,					   -2 / (f - n),					0,
			(r + l) / (l - r),			(t + b) / (b - t),		(f + n) / (n - f),				1
		};
	}

	// Create Orthographic matrix
	inline mat4 orthographic() {
		return	{
			1,			0,			0,			0,
			0,			1,			0,			0,
			0,			0,			0,			0,
			0,			0,		   -1,			1
		};
	}

	// Create Perspective matrix
	inline mat4 perspective(float dfov, float asr, float zn, float zf) {
		float f = 1 / tanf(radians(dfov / 2.0f));
		return	{
			f / asr,	0,		0,							 0,
			0,			f,		0,							 0,
			0,			0,		(zn + zf) / (zn - zf),		 2 * zn * zf / (zn - zf),
			0,			0,		-1,							 0
		};
	}

	// Perform perspective divide -> Divide x,y,z coords of each Vertex of Triangle by respective w value
	inline Tri perspectiveDivide(Tri it) {
		for (int i = 0; i < 3; i++)
		{
			it.vertex[i].position.x /= it.vertex[i].position.w;
			it.vertex[i].position.y /= it.vertex[i].position.w;
			it.vertex[i].position.z /= it.vertex[i].position.w;
			it.vertex[i].position.w = 1.0f;
		}
		return it;
	}

	// Normalize vector
	inline vec3 normalize(vec3 vi) {
		return vi.normalize();
	}
	// Magnitude of vector
	inline float length(vec3 vi) {
		return vi.magnitude();
	}
	// Dot Product
	inline float dot(vec3& v1, vec3& v2) {
		return v1.dot(v2);
	}
	// Cross Product
	inline vec3 cross(vec3& v1, vec3& v2) {
		return v1.cross(v2);
	}
	// Create Translation Matrix
	inline mat4 translate(vec3& vi) {
		return	{
			1,	0,	0,	vi.x,
			0,	1,	0,	vi.y,
			0,	0,	1,	vi.z,
			0,	0,	0,	1
		};
	}
	// Create Composite Rotation Matrix : for creating view matrix
	inline mat4 compositeRotate(vec3& u, vec3& v, vec3& n) {
		return	{
			u.x,	u.y,	u.z,	0,
			v.x,	v.y,	v.z,	0,
			n.x,	n.y,	n.z,	0,
			0,		0,		0,		1
		};
	}
	// Create Rotation about X Matrix
	inline mat4 rotatex(float x) {
		return	{
			1,	0,			0,			0,
			0,	cosf(x),	-sinf(x),	0,
			0,	sinf(x),	cosf(x) ,	0,
			0,	0,			0,			1
		};
	}
	// Create Rotation about Y Matrix
	inline mat4 rotatey(float x) {
		return	{
			cosf(x),	0,	-sinf(x),	0,
			0,			1,	0,			0,
			sinf(x),	0,	cosf(x) ,	0,
			0,			0,	0,			1
		};
	}
	// Create Rotation about Z Matrix
	inline mat4 rotatez(float x) {
		return	{
			cosf(x),	-sinf(x),	0,	0,
			sinf(x),	cosf(x),	0,	0,
			0,			0,			1 ,	0,
			0,			0,			0,	1
		};
	}
	//// Create Rotation about all axes Matrix
	inline mat4 rotate(vec3 vi) {
		return	{
			cosf(vi.y) * cosf(vi.z),										cosf(vi.y) * sinf(vi.z),										-sinf(vi.y),				0,
			sinf(vi.x) * sinf(vi.y) * cosf(vi.z) - cosf(vi.x) * sinf(vi.y), sinf(vi.x) * sinf(vi.y) * sinf(vi.z) + cosf(vi.x) * cosf(vi.z),	sinf(vi.x) * cosf(vi.y),	0,
			cosf(vi.x) * sinf(vi.y) * cosf(vi.z) + cosf(vi.x) * sinf(vi.z),	cosf(vi.x) * sinf(vi.y) * sinf(vi.z) - sinf(vi.x) * cosf(vi.y),	cosf(vi.x) * cosf(vi.y),		0,
			0,																0,																0,							1
		};
	}
	// Create Scaling Matrix
	inline mat4 scale(vec3& vi) {
		return	{
			vi.x,	0,		0,		0,
			0,		vi.y,	0,		0,
			0,		0,		vi.z,	0,
			0,		0,		0,		1
		};
	}
	// Create inverse of Camera Matrix: aka LookAt
	inline mat4 lookAt(Camera* c) {
		vec3 n(normalize(c->eye - c->target));		// negative forward direction of camera
		vec3 u(normalize(cross(c->up, n)));			// right
		vec3 v(normalize(cross(n, u)));				// new up
		return	{
			u.x,	u.y,	u.z,	-c->eye.dot(u),	// equivalent to translation followed by composite rotation
			v.x,	v.y,	v.z,	-c->eye.dot(v),
			n.x,	n.y,	n.z,	-c->eye.dot(n),
			0,		0,		0,		1
		};
	}
}