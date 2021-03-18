#pragma once

namespace constants
{
    //tiles
    const int EMPTY = 0;
    const int WALL = 1;
    const int START = 2;
    const int END = 3;
    const int VISITED = 4;
    const int PATH = 5;

    //color
    const int ALBEDO = 255;

    //screen
    const int SCREEN_WIDTH = 750;  //make sure dimensions are a multiple of and proportional to map dimensions
    const int SCREEN_HEIGHT = 450; //TODO: make into ratios and make cleaner

    //world
    const int MAP_WIDTH = 25;
    const int MAP_HEIGHT = 15;
    const int TILE_WIDTH = SCREEN_WIDTH / MAP_WIDTH;
    const int TILE_HEIGHT = SCREEN_HEIGHT / MAP_HEIGHT;

    //math
    const int MY_INFINITY = 10000000;
}