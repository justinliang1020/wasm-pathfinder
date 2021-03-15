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
    const int SCREEN_WIDTH = 900;  //make sure dimensions are a multiple of and proportional to map dimensions
    const int SCREEN_HEIGHT = 600; //TODO: make into ratios and make cleaner

    //world
    const int MAP_WIDTH = 30;
    const int MAP_HEIGHT = 20;
    const int TILE_WIDTH = SCREEN_WIDTH / MAP_WIDTH;
    const int TILE_HEIGHT = SCREEN_HEIGHT / MAP_HEIGHT;

}