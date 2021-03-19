#include <SDL2/SDL.h>
#include <emscripten.h>
#include <queue>
#include <vector>
#include <tuple>
#include "canvas.h"
#include "search.h"
#include "constants.h"
#include "maze.h"

using namespace constants;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

//screen
int SCREEN_WIDTH = 750;
int SCREEN_HEIGHT = 450;

int MAP_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
int MAP_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

//mouse paint event
bool mouse_painting = false;
bool mouse_dragging = false;
int mouse_tile = -1;
int mouse_loc_prev = -1;

//canvas rendering
canvas canv = canvas(MAP_WIDTH, MAP_HEIGHT);
std::queue<std::tuple<int, int>> render_queue;

// helper function for debugging in javascript console
EM_JS(void, take_args, (int x, int y), {
    console.log('I received: ' + [ x, y ]);
});
EM_JS(int, canvas_get_width, (), {
    return window.innerWidth;
});

EM_JS(int, canvas_get_height, (), {
    return window.innerHeight;
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

// interrupt a search animation if one is going on
bool check_animating()
{
    if (!render_queue.empty())
    {
        render_queue = {};
        canv.clear_search();
        render();
        return true;
    }
    else
    {
        canv.clear_search();
        render();
        return false;
    }
}

void checkDOM()
{
    int search_choice = EM_ASM_INT(
        return getSearch(););
    switch (search_choice)
    {
    case 0:
        if (!check_animating())
            breadth_first_search(canv, render_queue);
        break;
    case 1:
        if (!check_animating())
            depth_first_search(canv, render_queue);
        break;
    case 2:
        if (!check_animating())
            dijkstra(canv, render_queue);
        break;
    case 3:
        if (!check_animating())
            a_star(canv, render_queue);
        break;
    }
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
                mouse_tile = canv.at(mx, my);
                if (mouse_tile == 2 || mouse_tile == 3)
                    mouse_dragging = true;
                else
                    mouse_painting = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (mouse_dragging)
            {
                mouse_dragging = false;
                mouse_loc_prev = -1;
            }
            else
                mouse_painting = false;
            break;
        }
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_1:
                if (!check_animating())
                    breadth_first_search(canv, render_queue);
                break;
            case SDLK_2:
                if (!check_animating())
                    depth_first_search(canv, render_queue);
                break;
            case SDLK_3:
                if (!check_animating())
                    dijkstra(canv, render_queue);
                break;
            case SDLK_4:
                if (!check_animating())
                    a_star(canv, render_queue);
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
            case SDLK_SPACE:
                checkDOM();
                break;
                // case SDLK_r:
                //     if (!check_animating())
                //         recursive_division(canv, render_queue);
                //     break;
            }
        }
        //check if mouse isn't off window and if it isn't on a start/end tile
        if (mouse_painting && mx >= 0 && my >= 0 && canv.at(mx, my) != 2 && canv.at(mx, my) != 3)
        {
            if (mouse_tile == WALL)
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

        if (mouse_dragging && mx >= 0 && my >= 0)
        {
            // start cant drag to end and vice versa
            if (!((mouse_tile == START && canv.at(mx, my) == END) || (mouse_tile == END && canv.at(mx, my) == START)))
            {
                if (mouse_loc_prev != -1)
                    canv.paint(mouse_loc_prev, EMPTY);
                if (mouse_tile == START)
                {
                    canv.set_start(mx, my);
                }
                else
                {
                    canv.set_end(mx, my);
                }

                mouse_loc_prev = canv.convert(mx, my);
                render();
            }
        }
    }
}

void animation()
{
    if (!render_queue.empty())
    {
        int speed_choice = EM_ASM_INT(
            return getSpeed(););
        int animation_speed = 101 - speed_choice;
        std::tuple<int, int> frame = render_queue.front();
        render_queue.pop();
        int i = std::get<0>(frame);
        int color = std::get<1>(frame);
        if (canv.at(i) != START && canv.at(i) != END)
            canv.paint(i, color);
        render();
        SDL_Delay(animation_speed);
    }
}

void main_tick()
{
    get_input(event);
    animation();
}

int main()
{
    // gets width/height of browser window and scales SDL window accordingly
    int width = canvas_get_width();
    int height = canvas_get_height();
    take_args(width, height);
    SCREEN_WIDTH = (width - HORIZONTAL_MARGIN);
    SCREEN_WIDTH -= SCREEN_WIDTH % TILE_WIDTH;
    SCREEN_HEIGHT = (height - VERTICAL_MARGIN);
    SCREEN_HEIGHT -= SCREEN_HEIGHT % TILE_HEIGHT;
    MAP_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
    MAP_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;
    canv = canvas(MAP_WIDTH, MAP_HEIGHT);

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