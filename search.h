#pragma once
#include <queue>
#include <stack>
#include <unordered_set> //s
#include <vector>
#include <tuple>
#include "canvas.h"
#include "constants.h"

using std::queue;
using std::stack;
using std::tuple;
using std::unordered_set;
using std::vector;

void breadth_first_search(const int &start, const int &end, canvas &canv, queue<tuple<int, int>> &render_queue)
{
    queue<int> q;
    unordered_set<int> visited;
    vector<int> prev(canv.size());
    std::fill(prev.begin(), prev.end(), -1);

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        int tile = q.front();
        q.pop();
        render_queue.push(std::make_tuple(tile, VISITED));
        if (tile == end)
        {
            break;
        }
        for (int i : canv.adjacent(tile))
        {
            if (visited.find(i) == visited.end())
            {
                visited.insert(i);
                q.push(i);
                prev[i] = tile;
            }
        }
    }

    //reconstruct path
    vector<int> p;
    for (int i = end; i != -1; i = prev[i])
        p.push_back(i);

    //add path to render_queue in reverse
    for (auto i = p.rbegin(); i != p.rend(); ++i)
        render_queue.push(std::make_tuple(*i, PATH));
}

void depth_first_search(const int &start, const int &end, canvas &canv, queue<tuple<int, int>> &render_queue)
{
    stack<tuple<int, vector<int>>> s;
    vector<int> v;
    s.push(std::make_tuple(start, v));
    unordered_set<int> visited;

    while (!s.empty())
    {
        auto top = s.top();
        s.pop();
        int tile = std::get<0>(top);
        vector<int> cur_path = std::get<1>(top);
        render_queue.push(std::make_tuple(tile, VISITED));
        if (visited.find(tile) == visited.end())
        {
            if (tile == end)
            {
                v = cur_path;
                break;
            }
            visited.insert(tile);
            for (int i : canv.adjacent(tile))
            {
                vector<int> new_path = cur_path;
                new_path.push_back(i);
                s.push(std::make_tuple(i, new_path));
            }
        }
    }

    //add path to render_queue
    for (auto i = v.begin(); i != v.end(); ++i)
        render_queue.push(std::make_tuple(*i, PATH));
}