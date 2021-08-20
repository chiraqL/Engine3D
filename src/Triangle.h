#pragma once

#include "Vertex.h"
#include "Line.h"
#include "Texture.h"
#include "math/arithmetic.h"
#include "math/linear_algebra_vec.h"
#include "Timer.h"

struct Triangle
{
public:
	Triangle() {};
	Triangle(Vertex v1, Vertex v2, Vertex v3) {
		vertex[0] = v1;
		vertex[1] = v2;
		vertex[2] = v3;
	}
	Vertex vertex[3];

public:
	vec3 getNormal() {
		vec3 v01 = (vertex[1].position - vertex[0].position).tovec3();
		vec3 v02 = (vertex[2].position - vertex[0].position).tovec3();
		return ((v01.cross(v02)).normalize());
	}
	void getVertexNormals(vec3 norms[]) {
		for (int i = 0; i < 3; ++i)
			norms[i] = vertex[i].normal;
	}
	vec3 midPoint() {
		return	math::toVec3(vertex[0].position + vertex[1].position + vertex[2].position) / 3.0f;
	}
	void wireframe() {
		Line line;
		line.set(&vertex[0].position, &vertex[1].position); line.draw();	// 0 to 1
		line.set(&vertex[1].position, &vertex[2].position); line.draw();	// 1 to 2
		line.set(&vertex[2].position, &vertex[0].position); line.draw();	// 2 to 0
	}
	Triangle float_to_int() {
		Triangle otri = (*this);
		for (int i = 0; i < 3; i++)
		{
			otri.vertex[i].position.x = vertex[i].position.x * 0.5f * glb.draw.vp_width;
			otri.vertex[i].position.y = vertex[i].position.y * 0.5f * glb.draw.vp_height;
		}
		return otri;
	}
	col3 texAt(Texture& tex, float& i, float& j) {
		int x = i * tex.width;
		int y = j * tex.height;
		math::clamp(x, 0, tex.width -1);
		math::clamp(y, 0, tex.height - 1);
		int offset = (x + tex.width * y) * tex.nrComponents;
		int r = tex.data[offset + 0];
		int g = tex.data[offset + 1];
		int b = tex.data[offset + 2];
		return	{ r / 255.0f,g / 255.0f ,b / 255.0f };
	}

	Vertex vertexIntersectPlane(vec3& point_plane, vec3& normal_plane, Vertex& lineStart, Vertex& lineEnd, float& t)
	{
		Vertex vo;
		float plane_D = -normal_plane.dot(point_plane);
		float ad = lineStart.position.dot(math::toVec4(normal_plane));
		float bd = lineEnd.position.dot(math::toVec4(normal_plane));
		t = (-plane_D - ad) / (bd - ad);
		vo.position = lineStart.position + (lineEnd.position - lineStart.position) * t;
		vo.normal = lineStart.normal + (lineEnd.normal - lineStart.normal) * t;
		vo.tex = lineStart.tex + (lineEnd.tex - lineStart.tex) * t;
		return vo;
	}

	int triClipPlane(vec3 point_plane, vec3 normal_plane, Triangle& t1, Triangle& t2)
	{
		// Returns signed shortest-distance from point to plane
		auto distance = [&](vec3 p)
		{
			return (normal_plane.dot(p) - normal_plane.dot(point_plane));
		};

		Vertex* insideVertex[3]{}; int numInsidePoints = 0;
		Vertex* outsideVertex[3]{}; int numOutsidePoints = 0;

		// distance of each vertex to plane
		float d0 = distance(math::toVec3(this->vertex[0].position));
		float d1 = distance(math::toVec3(this->vertex[1].position));
		float d2 = distance(math::toVec3(this->vertex[2].position));

		if (d0 >= 0) { insideVertex[numInsidePoints++] = &this->vertex[0]; }
		else { outsideVertex[numOutsidePoints++] = &this->vertex[0]; }

		if (d1 >= 0) { insideVertex[numInsidePoints++] = &this->vertex[1]; }
		else { outsideVertex[numOutsidePoints++] = &this->vertex[1]; }

		if (d2 >= 0) { insideVertex[numInsidePoints++] = &this->vertex[2]; }
		else { outsideVertex[numOutsidePoints++] = &this->vertex[2]; }

		if (numInsidePoints == 0)
		{
			return 0;
		}
		if (numInsidePoints == 3)
		{
			t1 = *this;
			return 1;
		}
		if (numInsidePoints == 1 && numOutsidePoints == 2)
		{
			t1 = *this;

			t1.vertex[0] = *insideVertex[0];

			float t{};
			t1.vertex[1] = vertexIntersectPlane(point_plane, normal_plane, *insideVertex[0], *outsideVertex[0], t);
			t1.vertex[2] = vertexIntersectPlane(point_plane, normal_plane, *insideVertex[0], *outsideVertex[1], t);

			return 1;
		}
		if (numInsidePoints == 2 && numOutsidePoints == 1)
		{
			t1 = *this;

			t1.vertex[0] = *insideVertex[0];
			t1.vertex[1] = *insideVertex[1];
			t2.vertex[0] = *insideVertex[1];

			float t{};
			t1.vertex[2] = vertexIntersectPlane(point_plane, normal_plane, *insideVertex[0], *outsideVertex[0], t);
			t2.vertex[1] = t1.vertex[2];
			t2.vertex[2] = vertexIntersectPlane(point_plane, normal_plane, *insideVertex[1], *outsideVertex[0], t);

			return 2;
		}
	}

	// Color fill -> Flat Shading
	void fillF(col3* col) {
		Triangle inputTri = (*this).float_to_int();

		vert* v0 = &inputTri.vertex[0];
		vert* v1 = &inputTri.vertex[1];
		vert* v2 = &inputTri.vertex[2];

		// Sort vertices
		if (v2->position.y < v0->position.y) { math::pswap(&v2, &v0); }
		if (v2->position.y < v1->position.y) { math::pswap(&v2, &v1); }
		if (v1->position.y < v0->position.y) { math::pswap(&v1, &v0); }

		// find sizes to reserve std::vector
		int dy01 = math::abs(v0->position.y - v1->position.y) + 0.5f;
		int dy02 = math::abs(v0->position.y - v2->position.y) + 0.5f;
		int dy12 = math::abs(v1->position.y - v2->position.y) + 0.5f;
		int dy012 = dy01 + dy12;

		std::vector<int>   x012, x02, x12; x012.reserve(dy012); x02.reserve(dy02); x12.reserve(dy12);
		std::vector<float> z012, z02, z12; z012.reserve(dy012); z02.reserve(dy02); z12.reserve(dy12);

		math::interpolate<int>(x012, v0->position.y, v0->position.x, v1->position.y, v1->position.x);
		math::interpolate<int>(x12, v1->position.y, v1->position.x, v2->position.y, v2->position.x);
		math::interpolate<int>(x02, v0->position.y, v0->position.x, v2->position.y, v2->position.x);

		math::interpolate<float>(z012, v0->position.y, 1.0f / v0->position.z, v1->position.y, 1.0f / v1->position.z);
		math::interpolate<float>(z12, v1->position.y, 1.0f / v1->position.z, v2->position.y, 1.0f / v2->position.z);
		math::interpolate<float>(z02, v0->position.y, 1.0f / v0->position.z, v2->position.y, 1.0f / v2->position.z);

		x012.pop_back();
		z012.pop_back();

		x012.insert(x012.end(), x12.begin(), x12.end());
		z012.insert(z012.end(), z12.begin(), z12.end());

		std::vector<int>* x_left, * x_right;
		std::vector<float>* z_left, * z_right;

		int m = x02.size() / 2;
		if (x02[m] < x012[m])
		{
			x_left = &x02;
			x_right = &x012;
			z_left = &z02;
			z_right = &z012;
		}
		else
		{
			x_left = &x012;
			x_right = &x02;
			z_left = &z012;
			z_right = &z02;
		}
		for (int y = v0->position.y; y < v2->position.y; y++)
		{
			int x_l = (*x_left)[y - v0->position.y];
			int x_r = (*x_right)[y - v0->position.y];
			int dxlr = math::abs(x_r - x_l) + 0.5f;

			std::vector<float> z_segment; z_segment.reserve(dxlr);

			math::interpolate<float>(z_segment, x_l, (*z_left)[y - v0->position.y], x_r, (*z_right)[y - v0->position.y]);

			for (int x = x_l; x <= x_r; x++)
			{
				float z = z_segment[x - x_l];
				glb.draw.putpixel(x, y, z, *col);
			}
		}
	}

	// Color fill -> Gouraud Shading
	void fillG(col3* col) {
		Triangle inputTri = (*this).float_to_int();

		vert* v0 = &inputTri.vertex[0];
		vert* v1 = &inputTri.vertex[1];
		vert* v2 = &inputTri.vertex[2];

		// Sort vertices
		if (v2->position.y < v0->position.y) { math::pswap(&v2, &v0); }
		if (v2->position.y < v1->position.y) { math::pswap(&v2, &v1); }
		if (v1->position.y < v0->position.y) { math::pswap(&v1, &v0); }

		int dy01 = math::abs(v0->position.y - v1->position.y) + 0.5f;
		int dy02 = math::abs(v0->position.y - v2->position.y) + 0.5f;
		int dy12 = math::abs(v1->position.y - v2->position.y) + 0.5f;
		int dy012 = dy01 + dy12;

		std::vector<int>   x012, x02, x12; x012.reserve(dy012); x02.reserve(dy02); x12.reserve(dy12);
		std::vector<float> z012, z02, z12; z012.reserve(dy012); z02.reserve(dy02); z12.reserve(dy12);
		std::vector<col3>  c012, c02, c12; c012.reserve(dy012); c02.reserve(dy02); c12.reserve(dy12);

		math::interpolate<int>(x012, v0->position.y, v0->position.x, v1->position.y, v1->position.x);
		math::interpolate<int>(x12, v1->position.y, v1->position.x, v2->position.y, v2->position.x);
		math::interpolate<int>(x02, v0->position.y, v0->position.x, v2->position.y, v2->position.x);

		math::interpolate<float>(z012, v0->position.y, 1.0f / v0->position.z, v1->position.y, 1.0f / v1->position.z);
		math::interpolate<float>(z12, v1->position.y, 1.0f / v1->position.z, v2->position.y, 1.0f / v2->position.z);
		math::interpolate<float>(z02, v0->position.y, 1.0f / v0->position.z, v2->position.y, 1.0f / v2->position.z);

		math::interpolateColor(c012, v0->position.y, col[0], v1->position.y, col[1]);
		math::interpolateColor(c12,  v1->position.y, col[1], v2->position.y, col[2]);
		math::interpolateColor(c02,  v0->position.y, col[0], v2->position.y, col[2]);

		x012.pop_back();
		z012.pop_back();
		c012.pop_back();

		x012.insert(x012.end(), x12.begin(), x12.end());
		z012.insert(z012.end(), z12.begin(), z12.end());
		c012.insert(c012.end(), c12.begin(), c12.end());

		std::vector<int>* x_left, * x_right;
		std::vector<float>* z_left, * z_right;
		std::vector<col3>* c_left, * c_right;

		int m = x02.size() / 2;
		if (x02[m] < x012[m])
		{
			x_left = &x02;
			x_right = &x012;
			z_left = &z02;
			z_right = &z012;
			c_left = &c02;
			c_right = &c012;
		}
		else
		{
			x_left = &x012;
			x_right = &x02;
			z_left = &z012;
			z_right = &z02;
			c_left = &c012;
			c_right = &c02;
		}
		for (int y = v0->position.y; y < v2->position.y; y++)
		{
			int x_l = (*x_left)[y - v0->position.y];
			int x_r = (*x_right)[y - v0->position.y];
			int dxlr = math::abs(x_r - x_l) + 0.5f;

			std::vector<float> z_segment; z_segment.reserve(dxlr);
			std::vector<col3> c_segment; c_segment.reserve(dxlr);

			math::interpolate<float>(z_segment, x_l, (*z_left)[y - v0->position.y], x_r, (*z_right)[y - v0->position.y]);
			math::interpolateColor(c_segment, x_l, (*c_left)[y - v0->position.y], x_r, (*c_right)[y - v0->position.y]);

			for (int x = x_l; x <= x_r; x++)
			{
				float z = z_segment[x - x_l];
				col3 colVal = c_segment[x - x_l];
				glb.draw.putpixel(x, y, z, colVal);
			}
		}
	}

	void texfillF(Texture& texture, float* intensity) {
		Triangle inputTri = (*this).float_to_int();

		vert* v0 = &inputTri.vertex[0];
		vert* v1 = &inputTri.vertex[1];
		vert* v2 = &inputTri.vertex[2];

		// Sort vertices
		if (v2->position.y < v0->position.y) { math::pswap(&v2, &v0); }
		if (v2->position.y < v1->position.y) { math::pswap(&v2, &v1); }
		if (v1->position.y < v0->position.y) { math::pswap(&v1, &v0); }

		int dy01 = math::abs(v0->position.y - v1->position.y) + 0.5f;
		int dy02 = math::abs(v0->position.y - v2->position.y) + 0.5f;
		int dy12 = math::abs(v1->position.y - v2->position.y) + 0.5f;
		int dy012 = dy01 + dy12;

		std::vector<int>   x012, x02, x12; x012.reserve(dy012); x02.reserve(dy02); x12.reserve(dy12);
		std::vector<float> z012, z02, z12; z012.reserve(dy012); z02.reserve(dy02); z12.reserve(dy12);
		std::vector<float> u012, u02, u12; u012.reserve(dy012); u02.reserve(dy02); u12.reserve(dy12);
		std::vector<float> v012, v02, v12; v012.reserve(dy012); v02.reserve(dy02); v12.reserve(dy12);

		math::interpolate<int> (x012, v0->position.y, v0->position.x, v1->position.y, v1->position.x);
		math::interpolate<int> (x12, v1->position.y, v1->position.x, v2->position.y, v2->position.x);
		math::interpolate<int> (x02, v0->position.y, v0->position.x, v2->position.y, v2->position.x);

		math::interpolate<float>(z012, v0->position.y, 1.0f / v0->position.z, v1->position.y, 1.0f / v1->position.z);
		math::interpolate<float>(z12, v1->position.y, 1.0f / v1->position.z, v2->position.y, 1.0f / v2->position.z);
		math::interpolate<float>(z02, v0->position.y, 1.0f / v0->position.z, v2->position.y, 1.0f / v2->position.z);

		math::interpolate<float>(u012, v0->position.y, v0->tex.x / v0->position.z, v1->position.y, v1->tex.x / v1->position.z);
		math::interpolate<float>(u12, v1->position.y, v1->tex.x / v1->position.z, v2->position.y, v2->tex.x / v2->position.z);
		math::interpolate<float>(u02, v0->position.y, v0->tex.x / v0->position.z, v2->position.y, v2->tex.x / v2->position.z);

		math::interpolate<float>(v012, v0->position.y, v0->tex.y / v0->position.z, v1->position.y, v1->tex.y / v1->position.z);
		math::interpolate<float>(v12, v1->position.y, v1->tex.y / v1->position.z, v2->position.y, v2->tex.y / v2->position.z);
		math::interpolate<float>(v02, v0->position.y, v0->tex.y / v0->position.z, v2->position.y, v2->tex.y / v2->position.z);

		x012.pop_back();
		z012.pop_back();
		u012.pop_back();
		v012.pop_back();

		x012.insert(x012.end(), x12.begin(), x12.end());
		z012.insert(z012.end(), z12.begin(), z12.end());
		u012.insert(u012.end(), u12.begin(), u12.end());
		v012.insert(v012.end(), v12.begin(), v12.end());

		std::vector<int>	*x_left, *x_right;
		std::vector<float>	*z_left, *z_right;
		std::vector<float>	*u_left, *u_right;
		std::vector<float>	*v_left, *v_right;

		int m = x02.size() / 2;
		if (x02[m] < x012[m])
		{
			x_left = &x02;
			x_right = &x012;
			z_left = &z02;
			z_right = &z012;
			u_left = &u02;
			u_right = &u012;
			v_left = &v02;
			v_right = &v012;
		}
		else
		{
			x_left = &x012;
			x_right = &x02;
			z_left = &z012;
			z_right = &z02;
			u_left = &u012;
			u_right = &u02;
			v_left = &v012;
			v_right = &v02;
		}
		for (int y = v0->position.y; y < v2->position.y; y++)
		{
			int x_l = (*x_left)[y - v0->position.y];
			int x_r = (*x_right)[y - v0->position.y];

			int dxlr = math::abs(x_r - x_l) + 0.5f;

			std::vector<float> z_segment; z_segment.reserve(dxlr);
			std::vector<float> u_segment; u_segment.reserve(dxlr);
			std::vector<float> v_segment; v_segment.reserve(dxlr);

			math::interpolate<float>(z_segment, x_l, (*z_left)[y - v0->position.y], x_r, (*z_right)[y - v0->position.y]);
			math::interpolate<float>(u_segment, x_l, (*u_left)[y - v0->position.y], x_r, (*u_right)[y - v0->position.y]);
			math::interpolate<float>(v_segment, x_l, (*v_left)[y - v0->position.y], x_r, (*v_right)[y - v0->position.y]);

			for (int x = x_l; x <= x_r; x++)
			{
				float z = z_segment[x - x_l];
				float u = u_segment[x - x_l] / z;
				float v = v_segment[x - x_l] / z;
				col3 texCol = texAt(texture, u, v) * intensity[0];
				glb.draw.putpixel(x, y, z, texCol);
			}
		}
	}

	void texfillG(Texture& texture, float* intensity) {
		Triangle inputTri = (*this).float_to_int();

		vert* v0 = &inputTri.vertex[0];
		vert* v1 = &inputTri.vertex[1];
		vert* v2 = &inputTri.vertex[2];

		// Sort vertices
		if (v2->position.y < v0->position.y) { math::pswap(&v2, &v0); }
		if (v2->position.y < v1->position.y) { math::pswap(&v2, &v1); }
		if (v1->position.y < v0->position.y) { math::pswap(&v1, &v0); }

		int dy01 = math::abs(v0->position.y - v1->position.y) + 0.5f;
		int dy02 = math::abs(v0->position.y - v2->position.y) + 0.5f;
		int dy12 = math::abs(v1->position.y - v2->position.y) + 0.5f;
		int dy012 = dy01 + dy12;

		std::vector<int>   x012, x02, x12; x012.reserve(dy012); x02.reserve(dy02); x12.reserve(dy12);
		std::vector<float> z012, z02, z12; z012.reserve(dy012); z02.reserve(dy02); z12.reserve(dy12);
		std::vector<float> u012, u02, u12; u012.reserve(dy012); u02.reserve(dy02); u12.reserve(dy12);
		std::vector<float> v012, v02, v12; v012.reserve(dy012); v02.reserve(dy02); v12.reserve(dy12);
		std::vector<float>  c012, c02, c12; c012.reserve(dy012); c02.reserve(dy02); c12.reserve(dy12);

		math::interpolate<int>(x012, v0->position.y, v0->position.x, v1->position.y, v1->position.x);
		math::interpolate<int>(x12, v1->position.y, v1->position.x, v2->position.y, v2->position.x);
		math::interpolate<int>(x02, v0->position.y, v0->position.x, v2->position.y, v2->position.x);

		math::interpolate<float>(z012, v0->position.y, 1.0f / v0->position.z, v1->position.y, 1.0f / v1->position.z);
		math::interpolate<float>(z12, v1->position.y, 1.0f / v1->position.z, v2->position.y, 1.0f / v2->position.z);
		math::interpolate<float>(z02, v0->position.y, 1.0f / v0->position.z, v2->position.y, 1.0f / v2->position.z);

		math::interpolate<float>(u012, v0->position.y, v0->tex.x / v0->position.z, v1->position.y, v1->tex.x / v1->position.z);
		math::interpolate<float>(u12, v1->position.y, v1->tex.x / v1->position.z, v2->position.y, v2->tex.x / v2->position.z);
		math::interpolate<float>(u02, v0->position.y, v0->tex.x / v0->position.z, v2->position.y, v2->tex.x / v2->position.z);

		math::interpolate<float>(v012, v0->position.y, v0->tex.y / v0->position.z, v1->position.y, v1->tex.y / v1->position.z);
		math::interpolate<float>(v12, v1->position.y, v1->tex.y / v1->position.z, v2->position.y, v2->tex.y / v2->position.z);
		math::interpolate<float>(v02, v0->position.y, v0->tex.y / v0->position.z, v2->position.y, v2->tex.y / v2->position.z);

		math::interpolate<float>(c012, v0->position.y, intensity[0], v1->position.y, intensity[1]);
		math::interpolate<float>(c12, v1->position.y,  intensity[1], v2->position.y, intensity[2]);
		math::interpolate<float>(c02, v0->position.y,  intensity[0], v2->position.y, intensity[2]);

		x012.pop_back();
		z012.pop_back();
		u012.pop_back();
		v012.pop_back();
		c012.pop_back();

		x012.insert(x012.end(), x12.begin(), x12.end());
		z012.insert(z012.end(), z12.begin(), z12.end());
		u012.insert(u012.end(), u12.begin(), u12.end());
		v012.insert(v012.end(), v12.begin(), v12.end());
		c012.insert(c012.end(), c12.begin(), c12.end());

		std::vector<int>* x_left, * x_right;
		std::vector<float>* z_left, * z_right;
		std::vector<float>* u_left, * u_right;
		std::vector<float>* v_left, * v_right;
		std::vector<float>* c_left, * c_right;

		int m = x02.size() / 2;
		if (x02[m] < x012[m])
		{
			x_left = &x02;
			x_right = &x012;
			z_left = &z02;
			z_right = &z012;
			u_left = &u02;
			u_right = &u012;
			v_left = &v02;
			v_right = &v012;
			c_left = &c02;
			c_right = &c012;
		}
		else
		{
			x_left = &x012;
			x_right = &x02;
			z_left = &z012;
			z_right = &z02;
			u_left = &u012;
			u_right = &u02;
			v_left = &v012;
			v_right = &v02;
			c_left = &c012;
			c_right = &c02;
		}
		for (int y = v0->position.y; y < v2->position.y; y++)
		{
			int x_l = (*x_left)[y - v0->position.y];
			int x_r = (*x_right)[y - v0->position.y];

			int dxlr = math::abs(x_r - x_l) + 1.5f;

			std::vector<float> z_segment; z_segment.reserve(dxlr);
			std::vector<float> u_segment; u_segment.reserve(dxlr);
			std::vector<float> v_segment; v_segment.reserve(dxlr);
			std::vector<float> c_segment; c_segment.reserve(dxlr);

			math::interpolate<float>(z_segment, x_l, (*z_left)[y - v0->position.y], x_r, (*z_right)[y - v0->position.y]);
			math::interpolate<float>(u_segment, x_l, (*u_left)[y - v0->position.y], x_r, (*u_right)[y - v0->position.y]);
			math::interpolate<float>(v_segment, x_l, (*v_left)[y - v0->position.y], x_r, (*v_right)[y - v0->position.y]);
			math::interpolate<float>(c_segment, x_l, (*c_left)[y - v0->position.y], x_r, (*c_right)[y - v0->position.y]);

			for (int x = x_l; x <= x_r; x++)
			{
				float z = z_segment[x - x_l];
				float u = u_segment[x - x_l] / z;
				float v = v_segment[x - x_l] / z;
				float intensityVal = c_segment[x - x_l];
				col3 texCol = texAt(texture, u, v) * intensityVal;
				glb.draw.putpixel(x, y, z, texCol);
			}
		}
	}
};

typedef Triangle Tri;
