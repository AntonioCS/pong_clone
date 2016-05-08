
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "pong.h"

#define BLACK 0, 0, 0
#define WHITE 255, 255, 255

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const Uint8 *key_states;

int main(int argc, char* args[]) {
    //The window we'll be rendering to
    SDL_Window* window = NULL;

    /*
    SDL_Rect player1 = {.x = 20, .y = 30, .w = 20, .h = 100};
    SDL_Rect player2 = {.x = 600, .y = 30, .w = 20, .h = 100};

    struct Ball ball = {
        .coords.x = 120,
        .coords.y = 30,
        .coords.w = 20,
        .coords.h = 20,
        .left = 0,
        .up = 0
    };
     */

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        Pong_Init(SCREEN_HEIGHT, SCREEN_HEIGHT);

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
                //Clear screen 
                SDL_SetRenderDrawColor(gRenderer, WHITE, 0xFF);
                SDL_RenderClear(gRenderer);

                key_states = SDL_GetKeyboardState(NULL);

                Pong_Handle(key_states);
                Pong_Draw(gRenderer);


                /*

              

                player_movement(&player1);
                player_movement(&player2);

                //Draw Rect
                SDL_SetRenderDrawColor(gRenderer, BLACK, 0xFF);
                SDL_RenderDrawRect(gRenderer, &player1);
                SDL_RenderDrawRect(gRenderer, &player2);

                move_ball(&ball, &player1, &player2);
                SDL_RenderDrawRect(gRenderer, &(ball.coords));*/
            }
            //Update Screen
            SDL_RenderPresent(gRenderer);
        }

    } else {
        printf("Did not enter\n");
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}