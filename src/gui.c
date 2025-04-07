#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdio.h>

#include "./include/gui.h"
#include "./include/cpu.h"
#include "./logging/log.h"

static SDL_Window *win = NULL;
static SDL_Renderer *rend = NULL;
static SDL_Event ev;

int pixSize, startX, startY;

uint32_t frameStart;
uint32_t frameCur;
// Gameboy actually runs at 59.7 fps, but implementing that exactly will be a
// little difficult
// Frames should be between 58.8 fps and 62.5 fps
const uint32_t frameTime = 1000/60;

static void setPixSize() {
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    int widPix = w/160;
    int heightPix = h/144;

    widPix = widPix ? widPix : 1;
    heightPix = heightPix ? heightPix : 1;

    if (widPix < heightPix) {
	pixSize = widPix;
	startX = 0;
	int extraHeight = h - (pixSize * 144);
	startY = extraHeight/2;
    } else {
	pixSize = heightPix;
	startY = 0;
	int extraWidth = w - (pixSize * 160);
	startX = extraWidth/2;
    }
}
void initGUI() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        logMessage("SDL Failed to init", ERROR);
        exit(-1);
    }

    win = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_RESIZABLE);

    rend = SDL_CreateRenderer(win, -1, 0);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    frameStart = SDL_GetTicks();

    setPixSize();
}

void frameDelay() {
    frameCur = SDL_GetTicks();
    uint32_t curTime = frameCur - frameStart;

    if (curTime < frameTime) {
	SDL_Delay(frameTime - curTime);
    }
    frameStart = SDL_GetTicks();
}

static void pollGUIEvents() {
    while (SDL_PollEvent(&ev) != 0) {
        switch (ev.type) {
            case SDL_QUIT:
		stopRunning();
	    case SDL_WINDOWEVENT:
		if (ev.window.event == SDL_WINDOWEVENT_RESIZED ||
			ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		    setPixSize();
		}
        }
    }
}


void renderFrame(uint8_t screen[144][160]) {
    pollGUIEvents();
    SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(rend);
    int i, j;
    SDL_Rect rect = {.w = pixSize, .h = pixSize, .x=startX, .y=startY};
    for (i = 0; i < 144; i++) {
	for (j = 0; j < 160; j++) {
	    SDL_SetRenderDrawColor(rend, screen[i][j], screen[i][j], screen[i][j], 0xff);
	    SDL_RenderFillRect(rend, &rect);
	    rect.x += pixSize;
	}
	rect.x = startX;
	rect.y += pixSize;
    }
    SDL_RenderPresent(rend);
}

void closeGUI() {
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

