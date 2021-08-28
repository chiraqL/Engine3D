#pragma once

#include <cmath>
#include <assert.h>
#include <iostream>

struct Vector2d
{
    float x, y;
    Vector2d operator - (Vector2d& b) {
        return { x - b.x, y - b.y };
    }
    Vector2d operator + (Vector2d b) {
        return { x + b.x, y + b.y };
    }
    Vector2d operator * (Vector2d& b) {
        return { x * b.x, y * b.y };
    }
    Vector2d operator * (float& b) {
        return { x * b, y * b };
    }
};
struct Vector3d
{
    float x, y, z;
    Vector3d() {};
    Vector3d(float x, float y, float z) :x(x), y(y), z(z) {};
    // overload cout
    friend std::ostream& operator<<(std::ostream& os, const Vector3d& vec);
    // subtract
    Vector3d operator - (Vector3d b) {
        return { x - b.x, y - b.y, z - b.z };
    }
    Vector3d operator - () {
        return { -x, -y, -z };
    }
    // add
    Vector3d operator + (Vector3d const& b) {
        return { x + b.x, y + b.y, z + b.z };
    }
    // scalar multiplication
    Vector3d operator * (float val) {
        return { x * val, y * val, z * val };
    }
    // scalar divide
    Vector3d operator / (float val) {
        return { x / val, y / val, z / val };
    }
    // dot product
    float dot(Vector3d& b) {
        return (x * b.x + y * b.y + z * b.z);
    }
    // cross product
    Vector3d cross(Vector3d& b) {
        return { y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x };
    }
    // magnitude
    float magnitude() {
        return sqrt(x * x + y * y + z * z);
    }
    // normalize
    Vector3d normalize() {
        float L = this->magnitude();
        return { x / L, y / L, z / L };
    }
    // position vector of point
    Vector3d positionVector() {
        return { x, y, z };
    }
};

struct Vector4d
{
    float x, y, z, w = 1.0f;

    Vector4d() {};
    Vector4d(Vector3d& vi) :x(vi.x), y(vi.y), z(vi.z), w(1.0f) {};
    Vector4d(float x, float y, float z) :x(x), y(y), z(z), w(1.0f) {};
    Vector4d(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {};

    Vector3d tovec3() {
        return	{ x, y, z };
    }

    // subtract
    Vector4d operator - (Vector4d const& b) {
        return { x - b.x, y - b.y, z - b.z, w - b.w };
    }
    // add
    Vector4d operator + (Vector4d const& b) {
        return { x + b.x, y + b.y, z + b.z, w + b.w };
    }
    // scalar multiplication
    Vector4d operator * (float val) {
        return { x * val, y * val, z * val, w };
    }
    // scalar divide
    Vector4d operator / (float val) {
        return { x / val, y / val, z / val, 1.0f };
    }
    // dot product
    float dot(Vector4d b) {
        return (x * b.x + y * b.y + z * b.z);
    }
    // cross product
    Vector4d cross(Vector4d& b) {
        return { y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x, 0.0f };
    }
    // magnitude
    float magnitude() {
        return sqrt(x * x + y * y + z * z);
    }
    // normalize
    Vector4d normalize() {
        float L = this->magnitude();
        return { x / L, y / L, z / L, w };
    }
    // position vector of point
    Vector4d positionVector() {
        return { x, y, z, 0.0f };
    }
};

typedef struct Vector2d  vec2;
typedef struct Vector2d  tex2;
typedef struct Vector3d  vec3;
typedef struct Vector4d  vec4;

static vec3 ORIGIN = { 0,0,0 };
static vec3 UP = { 0,1,0 };
static vec3 DOWN = { 0,-1,0 };
static vec3 LEFT = { -1,0,0 };
static vec3 RIGHT = { 1,0,0 };
static vec3 FORWARD = { 0,0,-1 };
static vec3 BACKWARD = { 0,0,1 };