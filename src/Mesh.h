#pragma once

#include <vector>

#include "Math.h"
#include "Global.h"
#include "Triangle.h"
#include "Camera.h"
#include "Light.h"

class Mesh {
public:
    std::vector<Tri>        tris;
    std::vector<Texture>    textures;
    col3                    ka, kd, ks;     //illumination parameters
    int                     ns;

    Mesh(std::vector<Tri> tris, std::vector<Texture> textures, col3 ka, col3 kd, col3 ks, int ns)
        :
        tris(tris),
        textures(textures),
        ka(ka),
        kd(kd),
        ks(ks),
        ns(ns)
    {
        init();
    };

    void getColor(col3* intensity, Camera* camera, Light* light, mat4& mv, Tri& tri, int& type) {
        if (type == FLAT)
        {
            vec3 n = tri.getNormal();
            vec3 v = math::normalize(camera->eye - tri.midPoint());

            // Ambient component
            intensity[0] = ka * light->ia;

            // Diffuse component
            //
            // sun
            if (light->sun->isVisible)
            {
                vec3 l = (-light->sun->direction);
                float n_dot_l = n.dot(l);
                if (n_dot_l > 0)
                    intensity[0] += kd * light->sun->id * n_dot_l;
            }
            // no specular for Flat shading
            /* directional light
            for (auto& dlight : light->dlights)
            {
                vec3 l = (-dlight.direction);
                float n_dot_l = n.dot(l);
                if (n_dot_l > 0)
                    intensity[0] += kd * dlight.id * n_dot_l;
                // no specular for Flat shading
            }
             Point light
             ...
            */
            math::clampColor(&intensity[0]);
            return;
        }
        else if (type == GOURAUD)
        {
            vec3 n[3]; tri.getVertexNormals(n);
            /* NOTE: this normal needs to be rotated and normalized!
               this transformed normal only works for uniform scaling
               The region of the modelview matrix that contains the orientation is the top left 3×3 submatrix
               https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
               https://lxjk.github.io/2017/10/01/Stop-Using-Normal-Matrix.html
            */
            for (int i = 0; i < 3; ++i)
                n[i] = math::normalize(math::toVec3(mv * math::toVec4(n[i])));

            vec3 v[3];
            for (int i = 0; i < 3; ++i)
                v[i] = math::normalize(camera->eye - math::toVec3(tri.vertex[i].position));

            // Ambient component
            for (int i = 0; i < 3; ++i)
                intensity[i] = ka * light->ia;

            // Diffuse component
            // sun
            if (light->sun->isVisible)
            {
                vec3 l = (-light->sun->direction);
                float n_dot_l[3] = { n[0].dot(l), n[1].dot(l), n[2].dot(l) };
                if (n_dot_l[0] > 0)
                {
                    for (int i = 0; i < 3; ++i)
                        intensity[i] += light->sun->id * kd * n_dot_l[i];
                    vec3 r[3];
                    for (int i = 0; i < 3; ++i)
                        r[i] = math::normalize(n[i] * 2 * n_dot_l[i] - l);
                    float r_dot_v[3] = { r[0].dot(v[0]), r[1].dot(v[1]), r[2].dot(v[2]) };
                    if (r_dot_v[0] > 0)
                        for (int i = 0; i < 3; ++i)
                            intensity[i] += light->sun->is * ks * pow(r_dot_v[i], ns);
                }
            }
            /* directional light
            for (auto& dlight : light->dlights)
            {
                vec3 l = (-dlight.direction);
                float n_dot_l[3] = { n[0].dot(l), n[1].dot(l), n[2].dot(l) };
                if (n_dot_l[0] > 0)
                {
                    for (int i = 0; i < 3; ++i)
                        intensity[i] += dlight.id * kd * n_dot_l[i];
                    vec3 r[3];
                    for (int i = 0; i < 3; ++i)
                        r[i] = math::normalize(n[i] * 2 * n_dot_l[i] - l);
                    float r_dot_v[3] = { r[0].dot(v[0]), r[1].dot(v[1]), r[2].dot(v[2]) };
                    if (r_dot_v[0] > 0)
                        for (int i = 0; i < 3; ++i)
                            intensity[i] += dlight.is * ks.r * kd * pow(r_dot_v[i], ns);
                }
            }
            // Point light
            // ...
            */
            for (int i = 0; i < 3; ++i)
                math::clampColor(&intensity[i]);
            return;
        }
    }

    void getIntensity(float* intensity, Camera* camera, Light* light, mat4& mv, Tri& tri, int& type) {
        if (type == FLAT)
        {
            vec3 n = tri.getNormal();
            vec3 v = math::normalize(camera->eye - tri.midPoint());

            // Ambient component
            intensity[0] = ka.r * light->ia;

            // Diffuse component
            //
            // sun
            if (light->sun->isVisible)
            {
                vec3 l = (-light->sun->direction);
                float n_dot_l = n.dot(l);
                if (n_dot_l > 0)
                    intensity[0] += kd.r * light->sun->id.r * n_dot_l;
            }
            // no specular for Flat shading
            /* directional light
            for (auto& dlight : light->dlights)
            {
                vec3 l = (-dlight.direction);
                float n_dot_l = n.dot(l);
                if (n_dot_l > 0)
                    intensity[0] += kd * dlight.id * n_dot_l;
                // no specular for Flat shading
            }
             Point light
             ...
            */
            //math::clampColor(&intensity[0]);
            return;
        }
        else if (type == GOURAUD)
        {
            vec3 n[3]; tri.getVertexNormals(n);
            /* NOTE: this normal needs to be rotated and normalized!
               this transformed normal only works for uniform scaling
               The region of the modelview matrix that contains the orientation is the top left 3×3 submatrix
               https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
               https://lxjk.github.io/2017/10/01/Stop-Using-Normal-Matrix.html
            */
            for (int i = 0; i < 3; ++i)
                n[i] = math::normalize(math::toVec3(mv * math::toVec4(n[i])));

            vec3 v[3];
            for (int i = 0; i < 3; ++i)
                v[i] = math::normalize(camera->eye - math::toVec3(tri.vertex[i].position));

            // Ambient component
            for (int i = 0; i < 3; ++i)
                intensity[i] = ka.r * light->ia;

            // Diffuse component
            // sun
            if (light->sun->isVisible)
            {
                vec3 l = (-light->sun->direction);
                float n_dot_l[3] = { n[0].dot(l), n[1].dot(l), n[2].dot(l) };
                if (n_dot_l[0] > 0)
                {
                    for (int i = 0; i < 3; ++i)
                        intensity[i] += light->sun->id.r * kd.r * n_dot_l[i];
                    vec3 r[3];
                    for (int i = 0; i < 3; ++i)
                        r[i] = math::normalize(n[i] * 2 * n_dot_l[i] - l);
                    float r_dot_v[3] = { r[0].dot(v[0]), r[1].dot(v[1]), r[2].dot(v[2]) };
                    if (r_dot_v[0] > 0)
                        for (int i = 0; i < 3; ++i)
                            intensity[i] += light->sun->is.r * ks.r * pow(r_dot_v[i], ns);
                }
            }
            //for (int i = 0; i < 3; ++i)
                //math::clampColor(&intensity[i]);
            return;
        }
    }

    void draw(Camera* camera, Light* light, mat4& mv, mat4& p)
    {
        for (auto& tri : tris)
        {
            Tri t = mv * tri;

            // Backface culling
            vec3 tnormal = t.getNormal();
            vec3 tview = -t.midPoint();//camera->eye - t.midPoint();

            if (tnormal.dot(tview) >= 0) //if frontface
            {
                // Illumination
                col3 color[3]{};
                float intensity[3]{};
                if (textures.size() != 0)
                    getIntensity(intensity, camera, light, mv, t, glb.shading.type);    //for textured triangles
                else
                    getColor(color, camera, light, mv, t, glb.shading.type);        //for colored triangles

                // Clipping here
                int nClippedTris = 0;
                Tri tClipped[2];
                nClippedTris = t.triClipPlane({ 0.0f, 0.0f, -0.01f }, { 0.0f, 0.0f, -1.0f }, tClipped[0], tClipped[1]);
                for (int i = 0; i < nClippedTris; i++)
                {
                    tClipped[i] = p * tClipped[i];
                    if (glb.controls.projectP)
                        tClipped[i] = math::perspectiveDivide(tClipped[i]);

                    switch (i)
                    {
                    case 0:
                        glb.draw.invBgCol = { 1.0f, 0.0f, 0.0f };
                        break;
                    case 1:
                        glb.draw.invBgCol = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 2:
                        glb.draw.invBgCol = { 0.0f, 0.0f, 1.0f };
                        break;
                    default:
                        break;
                    }
                    if (glb.controls.wireFrame)
                        tClipped[i].wireframe();
                    else if(textures.size() != 0)
                    {
                        switch (glb.shading.type)
                        {
                        case FLAT:
                            tClipped[i].texfillF(textures[0], intensity); break;
                        case GOURAUD:
                            tClipped[i].texfillG(textures[0], intensity); break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        switch (glb.shading.type)
                        {
                        case FLAT:
                            tClipped[i].fillF(color); break;
                        case GOURAUD:
                            tClipped[i].fillG(color); break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

private:
    void init() {

    }
};