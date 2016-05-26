
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "pong.h"

int main(int argc, char* args[]) {
    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window* window = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() == 0) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        bool quit = false;
        bool paused = false;
        SDL_Event e;

        Pong_Init(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
        Pong_SetRenderer(gRenderer);

        while (!quit) {
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
                Pong_Handle(SDL_GetKeyboardState(NULL));
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