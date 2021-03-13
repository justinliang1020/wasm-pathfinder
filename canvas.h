#pragma once
#include <vector>
#include <algorithm>

class canvas
{
public:
    canvas(int x, int y) : width(x), height(y), map(x * y) {}
    canvas(int x, int y, int s, int f) : width(x), height(y), map(x * y), start(s), end(f) {}

    int at(int x, int y)
    {
        return map[x + y * width];
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

public:
    int width;
    int height;
    int start;
    int end;

private:
    std::vector<int> map;
};