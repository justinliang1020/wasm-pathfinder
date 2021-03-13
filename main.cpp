#include <SDL2/SDL.h>
#include <emscripten.h>
#include <queue>
#include <set>
#include "canvas.h"
#include "search.h"

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

//canvas
canvas canv = canvas(MAP_WIDTH, MAP_HEIGHT);

// helper function for debugging in javascript console
EM_JS(void, take_args, (int x, int y), {
    console.log('I received: ' + [ x, y ]);
});

void breadth_first_search()
{
    std::queue<int> q;
    std::set<int> discovered;
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255); // light grey
    SDL_RenderClear(renderer);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            SDL_Rect tileRect = {x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
            int tile = canv.at(x, y);
            switch (canv.at(x, y))
            {
            case 1:
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // grey wall
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case 2:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green start
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case 3:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red end
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &tileRect);
        }
    }

    SDL_RenderPresent(renderer);
}

void get_input(SDL_Event e)
{
    while (SDL_PollEvent(&e))
    {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        mx /= TILE_WIDTH;
        my /= TILE_HEIGHT;

        switch (e.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            cur_tile = canv.at(mx, my);
            painting = true;
            break;
        case SDL_MOUSEBUTTONUP:
            painting = false;
            break;
        }
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_q:
                canv.clear();
                render();
                break;
            case SDLK_w:
                canv.set_start(mx, my);
                render();
                break;
            case SDLK_e:
                canv.set_end(mx, my);
                render();
                break;
            }
        }
        if (painting && mx >= 0 && my >= 0) //check if mouse isn't off window
        {
            if (cur_tile == 1)
            {
                canv.paint(mx, my, 0);
                render();
            }
            else
            {
                canv.paint(mx, my, 1);
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

    canv.set_default_points();

    render();

    emscripten_set_main_loop(main_tick, -1, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}