#include "boiler.c"
#include "sand.c"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#define WIDTH 160
#define HEIGHT 120
#define SCALE 6

int main() {
    AppContext ctx = spawn_window(SCALE, WIDTH, HEIGHT);
    UIState ui = {
        .mouse_x = 0,
        .mouse_y = 0,
        .running = SDL_TRUE,
        .draw_radius = 1,
        .drawing = SDL_FALSE,
        .sim_paused = SDL_FALSE,
    };
    Particle grid[WIDTH][HEIGHT] = {0};

    while (ui.running) {
        SDL_Event event;
        SDL_PumpEvents();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ui.running = SDL_FALSE;
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    ui.sim_paused = !(ui.sim_paused);
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    ui.drawing = SDL_TRUE;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    ui.drawing = SDL_FALSE;
                }
            }

            if (event.type == SDL_MOUSEMOTION) {
                ui.mouse_x = event.motion.x / SCALE;
                ui.mouse_y = event.motion.y / SCALE;

            }

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    if (ui.draw_radius < 64) {ui.draw_radius++;}
                } else {
                    if (ui.draw_radius > 1) {ui.draw_radius--;}
                }
            }
        }

        if (ui.drawing) {
            for (int x = -ui.draw_radius; x <= ui.draw_radius; x++) {
                for (int y = -ui.draw_radius; y <= ui.draw_radius; y++) {
                    int posx = ui.mouse_x + x;
                    int posy = ui.mouse_y + y;

                    if (posx >= 0 && posy >= 0 && posx < WIDTH && posy < HEIGHT) {
                        if ((x * x + y * y) < (ui.draw_radius * ui.draw_radius)) {
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

        SDL_SetRenderDrawColor(ctx.render, 8, 16, 20, 255);
        SDL_RenderClear(ctx.render);

        if (!ui.sim_paused) {
            execute_simulation_step(WIDTH, HEIGHT, grid);
        }
        draw_grid(ctx.render, WIDTH, HEIGHT, grid);
        SDL_RenderPresent(ctx.render);

        SDL_Delay(1000.0 / 60);
    }
}
