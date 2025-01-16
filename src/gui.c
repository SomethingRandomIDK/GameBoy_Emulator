#include <SDL2/SDL.h>

#include "./include/gui.h"
#include "./logging/log.h"

static SDL_Window *win = NULL;
static SDL_Surface *surf = NULL;

void initGUI() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        logMessage("SDL Failed to init", ERROR);
        exit(-1);
    }

    win = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 1024, 0);

    surf = SDL_GetWindowSurface(win);
}

void closeGUI() {
    SDL_DestroyWindow(win);
    SDL_Quit();
}

