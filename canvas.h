#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "constants.h"

/* Tiles:
    0 - Empty
    1 - Wall
    2 - Start
    3 - End
    4 - Visited search tile
    5 - Final search path
*/

using namespace constants;

class canvas
{
public:
    canvas(int x, int y) : width(x), height(y), map(x * y) {}
    canvas(int x, int y, int s, int f) : width(x), height(y), map(x * y), start(s), end(f) {}

    int convert(int x, int y)
    {
        return x + y * width;
    }

    int convert_x(int i)
    {
        return i % width;
    }

    int convert_y(int i)
    {
        return i / width;
    }

    int size()
    {
        return width * height;
    }

    int at(int i)
    {
        return map[i];
    }

    int at(int x, int y)
    {
        return map[convert(x, y)];
    }

    // Returns adjacent locations (not including walls)
    std::vector<int> adjacent(int i)
    {
        std::vector<int> res;
        int x = convert_x(i);
        int y = convert_y(i);
        //up
        if (y - 1 >= 0 && at(x, y - 1) != WALL)
            res.push_back(convert(x, y - 1));
        //down
        if (y + 1 < height && at(x, y + 1) != WALL)
            res.push_back(convert(x, y + 1));
        //left
        if (x - 1 >= 0 && at(x - 1, y) != WALL)
            res.push_back(convert(x - 1, y));
        //right
        if (x + 1 < width && at(x+1, y) != WALL)
            res.push_back(convert(x + 1, y));
        return res;
    }

    void paint(int i, int color)
    {
        map[i] = color;
    }

    void paint(int x, int y, int color)
    {
        map[x + y * width] = color;
    }

    void clear()
    {
        std::fill(map.begin(), map.end(), 0);
        set_default_points();
    }

    void set_default_points()
    {
        start = width / 4 + (height / 2) * width;   // set start to left middle
        end = width * 3 / 4 + (height / 2) * width; // set end to right middle
        map[start] = 2;
        map[end] = 3;
    }

    void set_start(int x, int y)
    {
        map[start] = 0;
        start = x + y * width;
        paint(x, y, 2);
    }

    void set_end(int x, int y)
    {
        map[end] = 0;
        end = x + y * width;
        paint(x, y, 3);
    }

    // calculates the distances squared between points a and b
    int dist_squared(int a, int b)
    {
        int x1 = convert_x(a);
        int y1 = convert_y(a);
        int x2 = convert_x(b);
        int y2 = convert_y(b);

        return std::abs(y2 - y1) + std::abs(x2 - x1);
    }

public:
    int width;
    int height;
    int start;
    int end;

private:
    std::vector<int> map;
};