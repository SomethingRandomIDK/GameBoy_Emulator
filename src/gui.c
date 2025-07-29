#include <stdio.h>

#include <SDL.h>

#include "./include/gui.h"
#include "./include/cpu.h"
#include "./include/interrupt.h"
#include "./logging/log.h"

// This means that the events will be checked twice each frame
// A frame is about 70224 cycles

#define GUI_CYCLES 35112

static uint32_t guiCycleCount = 0;

static SDL_Window *win = NULL;
static SDL_Renderer *rend = NULL;
static SDL_GameController* cont = NULL;
static SDL_Event ev;
static SDL_AudioDeviceID aud;

static bool contConnected = false;

int pixSize, startX, startY;

float soundBuffer [4096] = {0};
int soundIdx = 0;

uint32_t frameStart;
uint32_t frameCur;
// Gameboy actually runs at 59.7 fps, but implementing that exactly will be a
// little difficult
// Frames should be between 58.8 fps and 62.5 fps
const uint32_t frameTime = 1000/60;

static uint8_t joypadMode = 0;

struct {
    bool a;
    bool b;
    bool start;
    bool select;
    bool up;
    bool down;
    bool left;
    bool right;
} buttons;

static void setPixSize() {
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    int widPix = w/160;
    int heightPix = h/144;

    widPix = widPix ? widPix : 1;
    heightPix = heightPix ? heightPix : 1;

    if (widPix < heightPix) {
        pixSize = widPix;
    } else {
        pixSize = heightPix;
    }
    int extraHeight = h - (pixSize * 144);
    startY = extraHeight/2;
    int extraWidth = w - (pixSize * 160);
    startX = extraWidth/2;
}
void initGUI() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO)) {
        logMessage("SDL Failed to init", ERROR);
        exit(-1);
    }

    win = SDL_CreateWindow("Gameboy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_RESIZABLE);

    rend = SDL_CreateRenderer(win, -1, 0);
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);

    frameStart = SDL_GetTicks();

    setPixSize();

    int nJoysticks = SDL_NumJoysticks();
    int i;
    for (i = 0; i < nJoysticks; i++) {
	if (SDL_IsGameController(i)) {
	    cont = SDL_GameControllerOpen(i);
	    contConnected = true;
	    break;
	}
    }

    SDL_AudioSpec specs = {
        .freq = GUI_FREQUENCY,
        .format = AUDIO_F32SYS,
        .channels = 2,
        .samples = SAMPLE_SIZE,
        .callback = NULL
    };

    aud = SDL_OpenAudioDevice(NULL, 0, &specs, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
    SDL_PauseAudioDevice(aud, 0);
}

void pushAudio(uint8_t sPanning, uint8_t mVol, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4){
    float buff0 = 0, buff1 = 0;
    int lVol = (mVol >> 4) & 0x7;
    int rVol = mVol & 0x7;

    lVol = (128 * lVol) / 7;
    rVol = (128 * rVol) / 7;

    if (sPanning & 0x80) {
        buff1 = ((float) ch4) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), lVol);
    }
    if (sPanning & 0x40) {
        buff1 = ((float) ch3) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), lVol);
    }
    if (sPanning & 0x20) {
        buff1 = ((float) ch2) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), lVol);
    }
    if (sPanning & 0x10) {
        buff1 = ((float) ch1) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), lVol);
    }

    soundBuffer[soundIdx] = buff0;
    soundIdx++;

    buff0 = 0;
    if (sPanning & 0x08) {
        buff1 = ((float) ch4) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), rVol);
    }
    if (sPanning & 0x04) {
        buff1 = ((float) ch3) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), rVol);
    }
    if (sPanning & 0x02) {
        buff1 = ((float) ch2) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), rVol);
    }
    if (sPanning & 0x01) {
        buff1 = ((float) ch1) / 64;
        SDL_MixAudioFormat((Uint8 *)&buff0, (Uint8 *)&buff1, AUDIO_F32SYS, sizeof(float), rVol);
    }


    soundBuffer[soundIdx] = buff0;
    soundIdx++;

    if (soundIdx > 1023) {
        soundIdx = 0;

        uint32_t buffSize = 1024 * sizeof(float);

        while (SDL_GetQueuedAudioSize(aud) > buffSize) {
        }

        SDL_QueueAudio(aud, soundBuffer, buffSize);
    }
}

static void frameDelay() {
    frameCur = SDL_GetTicks();
    uint32_t curTime = frameCur - frameStart;

    if (curTime < frameTime) {
        // printf("WAIT TIME: %d\n", frameTime - curTime);
	SDL_Delay(frameTime - curTime);
    }
    frameStart = SDL_GetTicks();
}

static void pollGUIEvents() {
    while (SDL_PollEvent(&ev) != 0) {
        switch (ev.type) {
            case SDL_QUIT:
		stopRunning();
		break;
	    case SDL_WINDOWEVENT:
		if (ev.window.event == SDL_WINDOWEVENT_RESIZED ||
			ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		    setPixSize();
		}
		break;
	    case SDL_KEYDOWN:
		switch(ev.key.keysym.sym) {
		    case SDLK_UP:
			buttons.up = true;
			if (!(joypadMode & 0x1)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_DOWN:
			buttons.down = true;
			if (!(joypadMode & 0x1)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_LEFT:
			buttons.left = true;
			if (!(joypadMode & 0x1)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_RIGHT:
			buttons.right = true;
			if (!(joypadMode & 0x1)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_q:
			buttons.a = true;
			if (!(joypadMode & 0x2)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_s:
			buttons.b = true;
			if (!(joypadMode & 0x2)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_TAB:
			buttons.select = true;
			if (!(joypadMode & 0x2)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		    case SDLK_RETURN:
			buttons.start = true;
			if (!(joypadMode & 0x2)) {
			    raiseInterrupt(JOYPAD);
			}
			break;
		}
		break;
	    case SDL_KEYUP:
		switch(ev.key.keysym.sym) {
		    case SDLK_UP:
			buttons.up = false;
			break;
		    case SDLK_DOWN:
			buttons.down = false;
			break;
		    case SDLK_LEFT:
			buttons.left = false;
			break;
		    case SDLK_RIGHT:
			buttons.right = false;
			break;
		    case SDLK_q:
			buttons.a = false;
			break;
		    case SDLK_s:
			buttons.b = false;
			break;
		    case SDLK_TAB:
			buttons.select = false;
			break;
		    case SDLK_RETURN:
			buttons.start = false;
			break;
		}
		break;
	    case SDL_CONTROLLERBUTTONDOWN:
		if (contConnected && ev.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(cont))) {
		    switch(ev.cbutton.button) {
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
			    buttons.up = true;
			    if (!(joypadMode & 0x1)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			    buttons.down = true;
			    if (!(joypadMode & 0x1)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			    buttons.left = true;
			    if (!(joypadMode & 0x1)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			    buttons.right = true;
			    if (!(joypadMode & 0x1)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_A:
			    buttons.a = true;
			    if (!(joypadMode & 0x2)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_B:
			    buttons.b = true;
			    if (!(joypadMode & 0x2)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_BACK:
			    buttons.select = true;
			    if (!(joypadMode & 0x2)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
			case SDL_CONTROLLER_BUTTON_START:
			    buttons.start = true;
			    if (!(joypadMode & 0x2)) {
				raiseInterrupt(JOYPAD);
			    }
			    break;
		    }
		}
		break;
	    case SDL_CONTROLLERBUTTONUP:
		if (contConnected && ev.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(cont))) {
		    switch(ev.cbutton.button) {
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
			    buttons.up = false;
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			    buttons.down = false;
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			    buttons.left = false;
			    break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			    buttons.right = false;
			    break;
			case SDL_CONTROLLER_BUTTON_A:
			    buttons.a = false;
			    break;
			case SDL_CONTROLLER_BUTTON_B:
			    buttons.b = false;
			    break;
			case SDL_CONTROLLER_BUTTON_BACK:
			    buttons.select = false;
			    break;
			case SDL_CONTROLLER_BUTTON_START:
			    buttons.start = false;
			    break;
		    }
		}
		break;
	    case SDL_CONTROLLERDEVICEADDED:
		if (!contConnected) {
		    if (SDL_IsGameController(ev.cdevice.which)) {
			cont = SDL_GameControllerOpen(ev.cdevice.which);
			contConnected = true;
		    }
		}
		break;
	    case SDL_CONTROLLERDEVICEREMOVED:
		if (contConnected) {
		    if (ev.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(cont))) {
			SDL_GameControllerClose(cont);
			cont = NULL;
			contConnected = false;
		    }
		}
		break;
        }
    }
}

bool checkStop() {
    return buttons.a || buttons.b || buttons.up || buttons.down || buttons.left || buttons.right || buttons.select || buttons.start;
}

uint8_t readJoypad() {
    uint8_t joyOutput = ((joypadMode << 4) & 0x30);
    joyOutput |= 0xf;
    if (!(joypadMode & 0x1)) {
        joyOutput &= ~(buttons.down << 3);
        joyOutput &= ~(buttons.up << 2);
        joyOutput &= ~(buttons.left << 1);
        joyOutput &= ~(buttons.right);
    }
    if (!(joypadMode & 0x2)) {
        joyOutput &= ~(buttons.start << 3);
        joyOutput &= ~(buttons.select << 2);
        joyOutput &= ~(buttons.b << 1);
        joyOutput &= ~(buttons.a);
    }
    return joyOutput;
}

void writeJoypad(uint8_t val) {
    joypadMode = ((val >> 4) & 0x3);
    uint8_t joyOutput = 0x0f;
    if (!(joypadMode & 0x1)) {
	joyOutput &= ~(buttons.down << 3);
	joyOutput &= ~(buttons.up << 2);
	joyOutput &= ~(buttons.left << 1);
	joyOutput &= ~(buttons.right);
    }
    if (!(joypadMode & 0x2)) {
	joyOutput &= ~(buttons.start << 3);
	joyOutput &= ~(buttons.select << 2);
	joyOutput &= ~(buttons.b << 1);
	joyOutput &= ~(buttons.a);
    }

    if ((joyOutput & 0x0f) != 0x0f) {
	raiseInterrupt(JOYPAD);
    }
}

void incEventTimer(uint32_t cycles) {
    guiCycleCount += cycles;
    if (guiCycleCount >= GUI_CYCLES) {
        guiCycleCount -= GUI_CYCLES;
        pollGUIEvents();
    }
}

uint32_t frameTimeStart = 0;
uint32_t frameCount = 0;

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

    uint32_t timePassed = SDL_GetTicks() - frameTimeStart;
    frameCount++;
    if (timePassed > 1000) {
        frameTimeStart = SDL_GetTicks();
        printf("FPS: %d\n", frameCount);
        printf("TIME: %d\n", timePassed);
        frameCount = 0;
    }
    // frameDelay();
}

void closeGUI() {
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

