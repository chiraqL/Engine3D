#pragma once

struct Color3d
{
    float r, g, b;
    Color3d& operator+=(const Color3d& col) {
        r += col.r;
        g += col.g;
        b += col.b;
        return *this;
    }
    Color3d& operator*=(const Color3d& col) {
        r *= col.r;
        g *= col.g;
        b *= col.b;
        return *this;
    }
    Color3d operator * (float intensity) {
        return { r * intensity, g * intensity , b * intensity };
    }
    Color3d operator * (Color3d& col) {
        return { r * col.r, g * col.g, b * col.b };
    }
    Color3d operator+(Color3d& col) {
        return  { r + col.r, g + col.g, b + col.b };
    }
    Color3d operator-(Color3d& col) {
        return  { r - col.r, g - col.g, b - col.b };
    }
    Color3d operator/(float val) {
        return  { r / val, g / val , b / val };
    }
};

struct Color4d
{
    float r, g, b, a = 1.0f;
    Color4d operator * (float& intensity) {
        return { r * intensity, g * intensity , b * intensity  ,1.0f };
    }
    Color4d operator-(Color4d& col) {
        return  { r - col.r, g - col.g, b - col.b };
    }
};

typedef struct Color3d col3;
typedef struct Color4d col4;

inline static col3 BLACK{ 0.0f,0.0f,0.0f };
inline static col3 WHITE{ 1.0f,1.0f,1.0f };