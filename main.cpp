#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cmath>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

//screen
const int SCREEN_WIDTH = 900;  //make sure dimensions are a multiple of and proportional to map dimensions
const int SCREEN_HEIGHT = 600; //TODO: make into ratios and make cleaner

//world
const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 20;
const int TILE_WIDTH = SCREEN_WIDTH / MAP_WIDTH;
const int TILE_HEIGHT = SCREEN_HEIGHT / MAP_HEIGHT;

//paint event
bool painting = false;
int cur_tile = -1;

int map[MAP_HEIGHT][MAP_WIDTH] = {};

void render()
{
    SDL_SetRenderDrawColor(renderer, 0x17, 0x2e, 0x1d, 255); //172e1d
    SDL_RenderClear(renderer);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            SDL_Rect tileRect = {x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
            if (map[y][x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 76, 72, 72, 255);
                SDL_RenderFillRect(renderer, &tileRect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tileRect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &tileRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void paint_tile(int x, int y, int color)
{
    map[y][x] = color;
}

void paint_mouse()
{
}

void get_input(SDL_Event e)
{
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            cur_tile = map[e.motion.y / TILE_HEIGHT][e.motion.x / TILE_WIDTH];
            painting = true;
            break;
        case SDL_MOUSEBUTTONUP:
            painting = false;
            break;
        }
        if (painting && e.motion.x > 0 && e.motion.y > 0)   //check if mouse isn't off window
        {
            if (cur_tile == 1)
                paint_tile(e.motion.x / TILE_WIDTH, e.motion.y / TILE_HEIGHT, 0);
            else
                paint_tile(e.motion.x / TILE_WIDTH, e.motion.y / TILE_HEIGHT, 1);
        }
    }
}

void main_tick()
{
    get_input(event);
    paint_mouse();
    render();
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    emscripten_set_main_loop(main_tick, -1, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}