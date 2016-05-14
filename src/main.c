
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "pong.h"

int main(int argc, char* args[]) {
    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    const Uint8 *key_states;

    SDL_Window* window = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() == 0) {
        //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        Pong_Init(SCREEN_HEIGHT, SCREEN_WIDTH);

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        bool paused = false;

        //While application is running
        while (!quit) {
            //Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                //User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_p:
                            paused = !paused;
                            break;
                    }
                }
            }

            if (paused == false) {
                key_states = SDL_GetKeyboardState(NULL);

                Pong_Handle(key_states);
                Pong_Draw(gRenderer);
            }
            //Update Screen
            SDL_RenderPresent(gRenderer);
        }

        SDL_DestroyRenderer(gRenderer);
    } else {
        printf("Unable to initialize SDL2\n");
    }

    SDL_DestroyWindow(window);

    Pong_Destroy();
    //Quit SDL subsystems
    TTF_Quit();
    SDL_Quit();

    return 0;
}