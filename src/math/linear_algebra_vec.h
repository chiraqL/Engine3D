#pragma once

#include "arithmetic.h"
#include "../Vector.h"
#include "../Color.h"

namespace math
{
	// convert spherical coordinate system to cartesian -> (r, phi, theta) to (x, y, z)
	/*
		z			   y
		|_y		->     |_x
	   /			  /
      x				 z
	*/
	inline vec4 sphToCart(vec3& sph) {
		vec4 cart;
		cart.x = sph.x * sinf(radians(sph.y)) * sinf(radians(sph.z)); // y = r sin(phi) sin(theta)
		cart.y = sph.x * cosf(radians(sph.y)); // z = r cos(phi)
		cart.z = sph.x * sinf(radians(sph.y)) * cosf(radians(sph.z)); // x = r sin(phi) cos(theta)
		cart.w = 1.0f;
		return cart;
	}
	// convert cartesian coordinate system to spherical -> (x, y, z) to (r, phi, theta)
	inline vec3 cartToSph(vec4& cart) {
		vec3 sph;
		sph.x = sqrtf(cart.z * cart.z + cart.x * cart.x + cart.y * cart.y); // r = sqrt(x^2 + y^2 + z^2)
		sph.y = degree(atan2f(sqrtf(cart.z * cart.z + cart.x * cart.x), cart.y));	// phi = arctan(sqrt(x^2 + y^2) / z)
		sph.z = degree(atan2f(cart.x, cart.z)); // theta = arctan(y/x)
		return sph;
	}

	// Vec4 to Vec3 -> w removed
	inline vec3 toVec3(vec4 vi) {
		return{ vi.x, vi.y, vi.z };
	}

	// Vec3 to Vec4 ->  *** w = 0.0f ***
	inline vec4 toVec4(vec3 vi) {
		return { vi.x, vi.y, vi.z, 0.0f };
	}

	// Clamp rgb 0.0f to 1.0f
	inline void clampColor(col3* col)
	{
		clamp(col->r, 0.0f, 1.0f);
		clamp(col->g, 0.0f, 1.0f);
		clamp(col->b, 0.0f, 1.0f);
	}

	// Linear Interpolation aka Lerp
	// Interpolate color values from d0 to d1 through i0 to i1 -> i & d are independent and dependent variables : d = f(i)
	inline void interpolateColor(std::vector<col3>& values, int i0, col3 d0, int i1, col3 d1)
	{
		if (i0 == i1)
		{
			values.emplace_back(d0);
			return;
		}
		col3 a = (d1 - d0) / (i1 - i0);	//increment color value
		col3 d = d0;
		for (int i = i0; i <= i1; i++)
		{
			values.emplace_back(d);
			d = d + a;
		}
		return;
	}

	// Linear Interpolation aka Lerp
	// Interpolate normal vector from d0 to d1 through i0 to i1 -> i & d are independent and dependent variables : d = f(i)
	inline void interpolateVector(std::vector<vec3>& values, int i0, vec3 d0, int i1, vec3 d1)
	{
		if (i0 == i1)
		{
			values.emplace_back(d0);
			return;
		}
		vec3 a = (d1 - d0) / (i1 - i0);	//increment color value
		vec3 d = d0;
		for (int i = i0; i <= i1; i++)
		{
			values.emplace_back(d);
			d = d + a;
		}
		return;
	}
}