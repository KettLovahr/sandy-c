#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>

typedef struct AppContext {
    SDL_Window* win;
    SDL_Surface* surf;
    SDL_Renderer* render;

} AppContext;

AppContext spawn_window(int render_scale, int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Sandy C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Surface* surf = SDL_GetWindowSurface(win);
    SDL_Renderer* render = SDL_GetRenderer(win);
    SDL_RenderSetScale(render, render_scale, render_scale);

    AppContext ctx = {
        .win = win,
        .surf = surf,
        .render = render,
    };
    return ctx;
}

typedef struct Interface {
    SDL_bool running;
    int mouse_x;
    int mouse_y;
    SDL_bool drawing;
    int draw_radius;
} Interface;
