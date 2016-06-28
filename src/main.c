
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "pong.h"
#include "Menu.h"


void menu_item_new_game(void *data) {
    log_info("New game selected");

}

void menu_item_settings(void *data) {
    log_info("Settings selected");
}

void menu_item_quit(void *data) {
    log_info("Quit selected");
}

enum game_screen {
    menu,
    game
};

void *menu_items[] = {
  "New Game", menu_item_new_game,
  "Settings", menu_item_settings,
  "Quit", menu_item_quit,
  NULL
};

int main(int argc, char* args[]) {
    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window* window = NULL;
    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    enum game_screen screen = menu;
    //enum game_screen screen = game;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() == 0 && (IMG_Init(img_flags) & img_flags) == img_flags) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        bool quit = false;
        bool paused = false;
        SDL_Event e;

        Pong_Init(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
        Pong_SetRenderer(gRenderer);

        Menu_Container *main_menu = Menu_Init(gRenderer, menu_items); //screen);

        //GameEngine_Font *menu_names = GameEngine_Font_Init(gRenderer, "./resources/font/press-start/prstart.ttf", 20);
        const Uint8 *keys = NULL;

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
                        case SDLK_a:
                            screen = (screen == menu ? game : menu);
                            break;
                        default:
                            Menu_Handle(main_menu, e.key.keysym.sym);
                    }
                }
            }

            keys = SDL_GetKeyboardState(NULL);
            //*/
            switch (screen) {
                case menu:
                    //Clear screen
                    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
                    SDL_RenderClear(gRenderer);

                    Menu_Display(main_menu, SCREEN_WIDTH / 2 - 20, 200);
                    break;
                case game:
                    if (paused == false) {
                        Pong_Handle(keys);
                        Pong_Draw(gRenderer);
                    }
                    break;
            }
            //*/

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