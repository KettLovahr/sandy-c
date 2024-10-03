#include <SDL2/SDL_render.h>
enum ParticleTypes {
    NONE,
    SAND,
    ROCK,
};

#include <string.h>
typedef struct Particle {
    unsigned char type;
    unsigned char properties;
} Particle;

// Properties
// 0000 0000
// |       |
// |       `- freefall
// |
// |
// |
// |
// |
// |
// `-


void execute_simulation_step(int width, int height,
                             Particle grid[width][height]) {
    Particle temp[width][height];
    //memcpy(temp, grid, sizeof(Particle) * width * height);
    memset(temp, 0, sizeof(Particle) * width * height);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            switch(grid[x][y].type) {
                case 0:
                    break;
                case 1:
                    if (y < height - 1) {
                        if (grid[x][y + 1].type == 0) {
                            temp[x][y + 1] = grid[x][y];
                        } else if (grid[x + 1][y + 1].type == 0) {
                            temp[x + 1][y + 1] = grid[x][y];
                        } else if (grid[x - 1][y + 1].type == 0) {
                            temp[x - 1][y + 1] = grid[x][y];
                        } else {
                            temp[x][y] = grid[x][y];
                        }
                    } else {
                        temp[x][y] = grid[x][y];
                    }
                    break;
            }
        }
    }
    memcpy(grid, temp, sizeof(Particle) * width * height);
}

void draw_grid(SDL_Renderer* render, int width, int height,
        Particle grid[width][height]) { 
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            switch(grid[x][y].type) {
                case 0:
                    break;
                case 1:
                    SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
                    SDL_RenderDrawPoint(render, x, y);
                    break;
            }
        }
    }
}
