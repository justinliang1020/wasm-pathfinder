#include <SDL2/SDL.h>
#include <emscripten.h>
#include "canvas.h"

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

canvas canv = canvas(MAP_WIDTH, MAP_HEIGHT);

void render()
{
    SDL_SetRenderDrawColor(renderer, 0x17, 0x2e, 0x1d, 255); //172e1d
    SDL_RenderClear(renderer);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            SDL_Rect tileRect = {x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
            if (canv.at(x, y) == 1)
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

void get_input(SDL_Event e)
{
    while (SDL_PollEvent(&e))
    {
        int x = e.motion.x / TILE_WIDTH;
        int y = e.motion.y / TILE_HEIGHT;
        switch (e.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            cur_tile = canv.at(x, y);
            painting = true;
            break;
        case SDL_MOUSEBUTTONUP:
            painting = false;
            break;
        }
        if (painting && x > 0 && y > 0)   //check if mouse isn't off window
        {
            if (cur_tile == 1)
            {
                canv.paint(x, y, 0);
                render();
            }
            else
            {
                canv.paint(x, y, 1);
                render();
            }
        }
    }
}

void main_tick()
{
    get_input(event);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    render();

    emscripten_set_main_loop(main_tick, -1, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}