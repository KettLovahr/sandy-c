#include <SDL2/SDL_render.h>
typedef enum ParticleTypes {
    NONE,
    SAND,
    ROCK,
    VOID = 255
} ParticleTypes;

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

ParticleTypes get_particle_type_at(int width, int height, Particle grid[width][height], int x, int y) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return VOID;
    } else {
        return grid[x][y].type;
    }
}

unsigned char get_particle_properties_at(int width, int height, Particle grid[width][height], int x, int y) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return 0x00;
    } else {
        return grid[x][y].properties;
    }
}

#define at(X, Y) get_particle_type_at(width, height, grid, (X), (Y))
#define prop(X, Y) get_particle_properties_at(width, height, grid, (X), (Y))

void execute_simulation_step(int width, int height,
                             Particle grid[width][height]) {
    Particle temp[width][height];
    //memcpy(temp, grid, sizeof(Particle) * width * height);
    memset(temp, 0, sizeof(Particle) * width * height);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Particle newpart = grid[x][y];
            switch(grid[x][y].type) {
                case 0:
                    break;
                case 1:
                    if (y < height - 1) {
                        if (at(x, y + 1) == 0 || (prop(x, y + 1) & 1) == 1) {
                            temp[x][y + 1] = newpart;
                            newpart.properties = newpart.properties | 0x01;
                        } else if (at(x + 1, y + 1) == 0) {
                            temp[x + 1][y + 1] = newpart;
                        } else if (at(x - 1, y + 1) == 0) {
                            temp[x - 1][y + 1] = newpart;
                        } else {
                            newpart.properties = newpart.properties & 0xFE;
                            temp[x][y] = newpart;
                        }
                    } else {
                        newpart.properties = newpart.properties & 0xFE;
                        temp[x][y] = newpart;
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
