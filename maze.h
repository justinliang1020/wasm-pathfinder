#pragma once

#include <stack>
#include <queue>
#include <tuple>
#include <vector>
#include <cmath>
#include <ctime>
#include "canvas.h"
#include "constants.h"


using std::stack;
using std::vector;
using std::queue;
using std::tuple;
using std::make_tuple;

//returns random odd int from (min,max) where min and max are even*
int random_int_odd(int min, int max)
{
    return min + 1 + 2 * (std::rand() % ((max - min) / 2));
}

//returns random even int from (min,max) where min and max are even*
int random_int_even(int min, int max)
{
    return min + 2 * (std::rand() % ((max - min) / 2));
}

void recursive_division_h(int x1, int y1, int x2, int y2, canvas &canv, queue<tuple<int, int>> &render_queue)
{
    if (x1 >= x2 - 2 || y1 >= y2 - 2)
        return;
    int hv = std::rand() % 2; //random either 0 or 1
    //int hv = 1;
    //horizontal line
    if (hv == 0)
    {
        int y3 = random_int_odd(y1, y2);
        for (int i = 0; i <= (x2 - x1); ++i)
        {
            render_queue.push(make_tuple(canv.convert(i, y3), WALL));
        }
        int entry = random_int_even(x1, x2);
        render_queue.push(make_tuple(canv.convert(entry, y3), EMPTY));
        //top recursion
        recursive_division_h(x1, y1, x2, y3 - 1, canv, render_queue);
        //bottom recursion
        recursive_division_h(x1, y3 + 1, x2, y2, canv, render_queue);
    }
    //vertical line
    else
    {
        int x3 = random_int_odd(x1, x2);
        for (int i = 0; i <= (y2 - y1); ++i)
        {
            render_queue.push(make_tuple(canv.convert(x3, i), WALL));
        }
        int entry = random_int_even(y1, y2);
        render_queue.push(make_tuple(canv.convert(x3, entry), EMPTY));
        //left recursion
        recursive_division_h(x1, y1, x3 - 1, y2, canv, render_queue);
        //right recursion
        recursive_division_h(x3 + 1, y1, x2 , y2, canv, render_queue);
    }
}

//ALGORITHM DOES NOT WORK

void recursive_division(canvas &canv, queue<tuple<int, int>> &render_queue)
{
    //make grid into 2n - 1 size grid
    //only place walls on odd numbered rows and columns (starting from index 0)
    //only place openings on even numberes rows and columns (starting from index 0)
    //sides must be odd length
    //render_queue.push(make_tuple(random_int_odd(0, 4), WALL));
    std::srand(std::time(nullptr));
    recursive_division_h(0, 0, canv.width - 1, canv.height - 1, canv, render_queue);
}