#include "boiler.c"
#include "sand.c"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#define WIDTH 640
#define HEIGHT 480

int main() {
    AppContext ctx = spawn_window(1, WIDTH, HEIGHT);
    Interface inter = {
        .mouse_x = 0,
        .mouse_y = 0,
        .running = SDL_TRUE,
        .draw_radius = 1,
        .drawing = SDL_FALSE,
    };
    Particle grid[WIDTH][HEIGHT] = {0};

    while (inter.running) {
        SDL_Event event;
        SDL_PumpEvents();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inter.running = SDL_FALSE;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    inter.drawing = SDL_TRUE;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    inter.drawing = SDL_FALSE;
                }
            }

            if (event.type == SDL_MOUSEMOTION) {
                inter.mouse_x = event.motion.x;
                inter.mouse_y = event.motion.y;

                if (inter.drawing) {
                    for (int x = -inter.draw_radius; x <= inter.draw_radius; x++) {
                        for (int y = -inter.draw_radius; y <= inter.draw_radius; y++) {
                            int posx = inter.mouse_x + x;
                            int posy = inter.mouse_y + y;

                            if (posx >= 0 && posy >= 0 && posx < WIDTH && posy < HEIGHT) {
                                if ((x * x + y * y) < (inter.draw_radius * inter.draw_radius)) {
                                    Particle newpart = {
                                        .type = 1,
                                        .properties = 0x01,
                                    };
                                    grid[posx][posy] = newpart;
                                }
                            }

                        }
                    }
                }
            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    if (inter.draw_radius < 64) {inter.draw_radius++;}
                } else {
                    if (inter.draw_radius > 1) {inter.draw_radius--;}
                }
            }
        }

        SDL_SetRenderDrawColor(ctx.render, 8, 16, 20, 255);
        SDL_RenderClear(ctx.render);

        execute_simulation_step(WIDTH, HEIGHT, grid);
        draw_grid(ctx.render, WIDTH, HEIGHT, grid);
        SDL_RenderPresent(ctx.render);

        SDL_Delay(1000.0 / 60);
    }
}
