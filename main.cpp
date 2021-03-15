#include <SDL2/SDL.h>
#include <emscripten.h>
#include <queue>         
#include <vector>
#include <tuple>
#include "canvas.h"
#include "search.h"
#include "constants.h"

using namespace constants;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

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

//paint event
bool painting = false;
int cur_tile = -1;

//canvas
canvas canv = canvas(MAP_WIDTH, MAP_HEIGHT);
std::queue<std::tuple<int, int>> render_queue;

// helper function for debugging in javascript console
EM_JS(void, take_args, (int x, int y), {
    console.log('I received: ' + [ x, y ]);
});

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
            case WALL:
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, ALBEDO); // grey wall
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case START:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, ALBEDO); // green start
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case END:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, ALBEDO); // red end
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case VISITED:
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, ALBEDO); // blue search
                SDL_RenderFillRect(renderer, &tileRect);
                break;
            case PATH:
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, ALBEDO); // orange search
                SDL_RenderFillRect(renderer, &tileRect);
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, ALBEDO);
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
            if (render_queue.empty())
            {
                cur_tile = canv.at(mx, my);
                painting = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            painting = false;
            break;
        }
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_1:
                if (!render_queue.empty())
                {
                    render_queue = {};
                    canv.clear();   //only clear visited and path tiles
                }
                else
                {
                    breadth_first_search(canv.start, canv.end, canv, render_queue);
                }
                render();
                break;
            case SDLK_q:
                if (!render_queue.empty())
                {
                    render_queue = {};
                }
                canv.clear();
                render();
                break;
            case SDLK_w:
                if (render_queue.empty())
                {
                    canv.set_start(mx, my);
                    render();
                }
                break;
            case SDLK_e:
                if (render_queue.empty())
                {
                    canv.set_end(mx, my);
                    render();
                }
                break;
            }
        }
        if (painting && mx >= 0 && my >= 0) //check if mouse isn't off window
        {
            if (cur_tile == WALL)
            {
                canv.paint(mx, my, EMPTY);
                render();
            }
            else
            {
                canv.paint(mx, my, WALL);
                render();
            }
        }
    }
}

void animation()
{
    if (!render_queue.empty())
    {
        std::tuple<int, int> frame = render_queue.front();
        render_queue.pop();
        int i = std::get<0>(frame);
        int color = std::get<1>(frame);
        if (canv.at(i) != START && canv.at(i) != END)
            canv.paint(i, color);
        render();
        SDL_Delay(20);
    }
}

void main_tick()
{
    get_input(event);
    animation();
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