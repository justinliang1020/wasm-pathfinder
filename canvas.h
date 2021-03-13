#pragma once
#include <vector>
#include <algorithm>

class canvas
{
public:
    canvas(int x, int y) : width(x), height(y), map(x * y) {}

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
        set_default();
    }

    void set_default()
    {
        map[width / 4 + (height / 2) * width] = 2;   // set start to left middle
        map[width * 3 / 4 + (height / 2) * width] = 3;   // set end to right middle
    }

public:
    int width;
    int height;

private:
    std::vector<int> map;
};