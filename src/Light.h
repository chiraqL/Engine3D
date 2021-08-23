#pragma once

#include <vector>
#include "math.h"
#include "Vector.h"
#include "Color.h"

struct DLight   // Directional light
{
public:
    col3 id, is;        // diffuse and specular
    vec4 position;
    vec3 direction;
public:
    DLight(col3 id, col3 is, vec4 position, vec3 direction)
        :
        id(id),
        is(is),
        position(position),
        direction(direction)
    {};
    ~DLight() {};
public:
    void setDirection(vec3& target, vec4& pos)
    {
        direction = math::normalize(target - math::toVec3(pos));
    }
};

struct Sun :public DLight
{
public:
    vec4 tfPosition;   // position after transformation (model transformation + light translation)
    vec3 translateSph; // spherical translation factors for sun -> (r, phi, theta) : phi(angle P with Y), theta(angle P' with Z)
    bool isVisible = true;
public:
    Sun(col3 id, col3 is, vec4 position, vec3 direction)
        :
        DLight(id, is, position, direction)
    {
        translateSph = math::cartToSph(position);
    };
};

struct PLight   // point light
{
    col3 id, is;
    vec4 position;
    float att;      //attenuation = 1/(ad^2+bd+c)
};

struct Light
{
public:
    float ia;   // ambient component of surrounding
    Sun* sun;
    std::vector<DLight> dlights;
    std::vector<PLight> plights;
public:
    Light() { ia = 1.0f; }  // default constructor
    Light(float ia)
        :
        ia(ia)
    {};
};
