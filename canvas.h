#pragma once
#include <vector>
#include <algorithm>

class canvas
{
public:
    canvas(int x, int y) : width(x), height(y), map(x * y) {}
    canvas(int x, int y, int s, int f) : width(x), height(y), map(x * y), start(s), end(f) {}

    int convert(int x, int y)
    {
        return x + y * width;
    }

    int at(int x, int y)
    {
        return map[convert(x, y)];
    }

    std::vector<int> adjacent(int i)
    {
        std::vector<int> res;
        int x = i % width;
        int y = i / width;
        //up
        if (y - 1 >= 0)
            res.push_back(convert(x, y - 1));
        //down
        if (y + 1 < height)
            res.push_back(convert(x, y + 1));
        //left
        if (x - 1 >= 0)
            res.push_back(convert(x - 1, y));
        //right
        if (x + 1 < width)
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

public:
    int width;
    int height;
    int start;
    int end;

private:
    std::vector<int> map;
};