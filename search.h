#pragma once
#include <queue>
#include <stack>
#include <unordered_set> //s
#include <vector>
#include <tuple>
#include <functional>
#include "canvas.h"
#include "constants.h"

using std::greater;
using std::make_tuple;
using std::priority_queue;
using std::queue;
using std::stack;
using std::tuple;
using std::unordered_set;
using std::vector;

void breadth_first_search(canvas &canv, queue<tuple<int, int>> &render_queue)
{
    const int start = canv.start;
    const int end = canv.end;
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
        render_queue.push(make_tuple(tile, VISITED));
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
        render_queue.push(make_tuple(*i, PATH));
}

void depth_first_search(canvas &canv, queue<tuple<int, int>> &render_queue)
{
    const int start = canv.start;
    const int end = canv.end;

    stack<tuple<int, vector<int>>> s;
    vector<int> v;
    s.push(make_tuple(start, v));
    unordered_set<int> visited;

    while (!s.empty())
    {
        auto top = s.top();
        s.pop();
        int tile = std::get<0>(top);
        vector<int> cur_path = std::get<1>(top);
        if (visited.find(tile) == visited.end())
        {
            render_queue.push(make_tuple(tile, VISITED));
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
                s.push(make_tuple(i, new_path));
            }
        }
    }

    //add path to render_queue
    for (auto i = v.begin(); i != v.end(); ++i)
        render_queue.push(make_tuple(*i, PATH));
}

void dijkstra(canvas &canv, queue<tuple<int, int>> &render_queue)
{
    const int start = canv.start;
    const int end = canv.end;

    vector<int> dist;
    priority_queue<tuple<int, int>, vector<tuple<int, int>>, greater<tuple<int, int>>> pq;
    unordered_set<int> visited;
    vector<int> prev(canv.size());
    int end_dist = MY_INFINITY;

    std::fill(prev.begin(), prev.end(), -1);

    for (int i = 0; i < canv.size(); ++i)
    {
        dist.push_back(MY_INFINITY);
    }

    pq.push(make_tuple(0, start));
    dist[start] = 0;

    while (!pq.empty())
    {
        int cur_dist = std::get<0>(pq.top());
        int tile = std::get<1>(pq.top());
        if (cur_dist > end_dist)
            break;
        if (tile == end)
        {
            end_dist = cur_dist;
            break;
        }
        pq.pop();
        for (int i : canv.adjacent(tile))
        {
            if (visited.find(i) == visited.end() && dist[i] > dist[tile] + 1)
            {
                dist[i] = dist[tile] + 1;
                pq.push(make_tuple(dist[i], i));
                prev[i] = tile;
            }
        }
        render_queue.push(make_tuple(tile, VISITED));
        visited.insert(tile);
    }

    //reconstruct path
    vector<int> p;
    for (int i = end; i != -1; i = prev[i])
        p.push_back(i);

    //add path to render_queue in reverse
    for (auto i = p.rbegin(); i != p.rend(); ++i)
        render_queue.push(make_tuple(*i, PATH));
}

// distance heuristic is distance + dist_squared
void a_star(canvas &canv, queue<tuple<int, int>> &render_queue)
{
    const int start = canv.start;
    const int end = canv.end;

    vector<int> dist;
    priority_queue<tuple<int, int>, vector<tuple<int, int>>, greater<tuple<int, int>>> pq;
    unordered_set<int> visited;
    vector<int> prev(canv.size());
    int end_dist = MY_INFINITY;

    std::fill(prev.begin(), prev.end(), -1);

    for (int i = 0; i < canv.size(); ++i)
    {
        dist.push_back(MY_INFINITY);
    }

    pq.push(make_tuple(canv.dist_squared(start, end), start));
    dist[start] = 0;

    while (!pq.empty())
    {
        int cur_dist = std::get<0>(pq.top());
        int tile = std::get<1>(pq.top());
        if (cur_dist > end_dist)
            break;
        if (tile == end)
        {
            end_dist = cur_dist;
            break;
        }
        pq.pop();
        for (int i : canv.adjacent(tile))
        {
            int weight = canv.dist_squared(i, end) + 1;
            if (visited.find(i) == visited.end() && dist[i] > weight)
            {
                dist[i] = weight;
                pq.push(make_tuple(weight, i));
                prev[i] = tile;
            }
        }
        render_queue.push(make_tuple(tile, VISITED));
        visited.insert(tile);
    }

    //reconstruct path
    vector<int> p;
    for (int i = end; i != -1; i = prev[i])
        p.push_back(i);

    //add path to render_queue in reverse
    for (auto i = p.rbegin(); i != p.rend(); ++i)
        render_queue.push(make_tuple(*i, PATH));
}