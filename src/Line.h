#pragma once

#include "Vector.h"
#include "Global.h"
#include "math/arithmetic.h"

class Line
{
public:
    Line() {}
	Line(vec4* v1, vec4* v2)
        :
        v1(v1),
        v2(v2)
	{};
	~Line() {};

private:
	vec4* v1,* v2;
public:
    void set(vec4* iv1, vec4* iv2) {
        v1 = iv1;
        v2 = iv2;
    }
    //Bresenham's line drawing algo
	void draw() {
        int		x, y;
        int		dx, dy;
        int		incx, incy;
        int		balance;

        if (v1->y > v2->y)
            std::swap(v1, v2);      // vertex v1 has lower y value

        // -1,1   to   1,-1     ->     -w/2, h/2   to  w/2, -h/2
        int     x1 = v1->x * 0.5f * glb.draw.vp_width;
        int     y1 = v1->y * 0.5f * glb.draw.vp_height;
        int     x2 = v2->x * 0.5f * glb.draw.vp_width;
        int     y2 = v2->y * 0.5f * glb.draw.vp_height;

        std::vector<float> z_segment;
        math::interpolate<float>(z_segment, y1, 1.0f / v1->z, y2, 1.0f / v2->z);    // interpolate z for zbuffer

        if (x2 >= x1)
        {
            dx = x2 - x1;
            incx = 1;
        }
        else
        {
            dx = x1 - x2;
            incx = -1;
        }

        if (y2 >= y1)
        {
            dy = y2 - y1;
            incy = 1;
        }
        else
        {
            dy = y1 - y2;
            incy = -1;
        }

        x = x1;
        y = y1;

        if (dx >= dy)
        {
            dy <<= 1;
            balance = dy - dx;
            dx <<= 1;

            while (x != x2)
            {
                glb.draw.putpixel(x, y, z_segment[y - y1], glb.draw.invBgCol);
                if (balance >= 0)
                {
                    y += incy;
                    balance -= dx;
                }
                balance += dy;
                x += incx;
            }
            glb.draw.putpixel(x, y, z_segment[y - y1], glb.draw.invBgCol);
        }
        else
        {
            dx <<= 1;
            balance = dx - dy;
            dy <<= 1;

            while (y != y2)
            {
                glb.draw.putpixel(x, y, z_segment[y - y1], glb.draw.invBgCol);
                if (balance >= 0)
                {
                    x += incx;
                    balance -= dy;
                }
                balance += dx;
                y += incy;
            }
            glb.draw.putpixel(x, y, z_segment[y - y1], glb.draw.invBgCol);
        }
	}
};