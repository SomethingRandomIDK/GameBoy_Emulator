#include <SDL2/SDL.h>

#include "./include/gui.h"
#include "./logging/log.h"

static SDL_Window *win = NULL;
static SDL_Renderer *rend = NULL;
static SDL_Event ev;


void initGUI() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        logMessage("SDL Failed to init", ERROR);
        exit(-1);
    }

    win = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_RESIZABLE);

    rend = SDL_CreateRenderer(win, -1, 0);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
}

void closeGUI() {
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

bool pollGUIEvents() {
    while (SDL_PollEvent(&ev) != 0) {
        switch (ev.type) {
            case SDL_QUIT:
                return false;
        }
    }
    return true;
}

