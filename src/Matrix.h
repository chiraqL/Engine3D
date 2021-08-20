#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "Triangle.h"

struct Matrix4d
{
    float m[4][4] = { 0 };
    // matrix multiply
    inline Matrix4d operator*(Matrix4d& mi) {
        Matrix4d mo;
        for (int i = 0; i < 4; i++)
            for (int k = 0; k < 4; k++)
                for (int j = 0; j < 4; j++)
                    mo.m[i][j] += this->m[i][k] * mi.m[k][j];
        return mo;
    }
    inline vec4 operator*(vec4 vi) {
        vec4 vo = vi;
        vo.x = vi.x * m[0][0] + vi.y * m[0][1] + vi.z * m[0][2] + vi.w * m[0][3];
        vo.y = vi.x * m[1][0] + vi.y * m[1][1] + vi.z * m[1][2] + vi.w * m[1][3];
        vo.z = vi.x * m[2][0] + vi.y * m[2][1] + vi.z * m[2][2] + vi.w * m[2][3];
        vo.w = vi.x * m[3][0] + vi.y * m[3][1] + vi.z * m[3][2] + vi.w * m[3][3];
        return vo;
    }
    inline vert operator*(vert& iv) {
        vert ov = iv;
        ov.position = (*this) * iv.position;
        return ov;
    }
    inline Tri operator*(Tri& it) {
        Tri ot;
        for (int i = 0; i < 3; i++)
            ot.vertex[i] = (*this) * it.vertex[i];
        return ot;
    }
    inline Matrix4d transpose(){
        Matrix4d mo;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mo.m[j][i] = m[i][j];
        return mo;
    };
};

typedef struct Matrix4d mat4;

#endif