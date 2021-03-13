#pragma once
#include <vector>

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

public:
    int width;
    int height;

private:
    std::vector<int> map;
};