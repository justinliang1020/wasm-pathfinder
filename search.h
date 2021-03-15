#pragma once
#include <queue>         
#include <unordered_set> //s
#include <vector>
#include <tuple>
#include "canvas.h"
#include "constants.h"

void breadth_first_search(const int& start, const int& end, canvas& canv, std::queue<std::tuple<int,int>>& render_queue)
{
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::vector<int> prev(canv.size());
    std::fill(prev.begin(), prev.end(), -1);

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        render_queue.push(std::make_tuple(node, VISITED));
        if (node == end)
        {
            break;
        }
        for (int i : canv.adjacent(node))
        {
            if (visited.find(i) == visited.end())
            {
                visited.insert(i);
                q.push(i);
                prev[i] = node;
            }
        }
    }

    //reconstruct path
    std::vector<int> p;
    for (int i = end; i != -1; i = prev[i])
        p.push_back(i);
    
    //add path to render_queue in reverse
    for (auto i = p.rbegin(); i != p.rend(); ++i)
        render_queue.push(std::make_tuple(*i, PATH));
}