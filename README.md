# wasm-pathfinder
[WebAssembly Pathfinder Visualizer](https://justinliang1020.github.io/wasm-pathfinder/)

Introduction
------------------

This is a pathfinding algorithm visualizer built for the web. Built using C++ with SDL2 and compiled to WebAssembly.

Algorithms:
1. Breadth-First Search
2. Depth-First Search
3. Dijkstra's Algorithm 
  - Gives pretty much the same visualization as BFS since it uses an unweighted graph
4. A* (-like) Search
  - Uses manhattan distance^2 as the guiding heuristic function
  - This heuristic is not consistent in giving an estimation smaller than the actual shortest path, meaning that this is only a "pseudo-A* search" algorithm and won't always draw the optimal path.

Building and Running
---------------------

1. [Install Emscripten] (https://emscripten.org/docs/getting_started/downloads.html)
2. [Install SDL2] (https://www.libsdl.org/download-2.0.php)
3. cd into project directory
4. Run: em++ main.cpp -o index_template.html --shell-file index.html -s USE_SDL=2
5. Run: python -m http.server
6. Visit [http://localhost:8000/](http://localhost:8000/)
