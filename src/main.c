
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define BLACK 0, 0, 0
#define WHITE 255, 255, 255

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int ball_speed = 2;
int player_speed = 10;
const Uint8 *key_states;

struct Ball {
    SDL_Rect coords;
    unsigned int left;
    unsigned int up;
};


bool checkCollision(SDL_Rect *obj1, SDL_Rect *obj2);

void move_ball(struct Ball *ball, SDL_Rect *p1, SDL_Rect *p2) {
    int speed = ball_speed;

    //moving to the right
    if (ball->left == 0) {
        if (!checkCollision(&(ball->coords), p2)) {
            ball->coords.x += speed;
        } else {
            ball->left = 1;
        }
    } else {
        if (!checkCollision(&(ball->coords), p1)) {
            ball->coords.x -= speed;
        } else {
            ball->left = 0;
        }
    }

    if (ball->up == 0) {
        ball->coords.y += speed;
    } else {
        ball->coords.y -= speed;
    }


    if (ball->coords.x <= 0) {
        ball->left = 0;
    } else if (ball->coords.x + ball->coords.w >= SCREEN_WIDTH) {
        ball->left = 1;
    }

    if (ball->coords.y <= 0) {
        ball->up = 0;
    } else if (ball->coords.y + ball->coords.h >= SCREEN_HEIGHT) {
        ball->up = 1;
    }
}

void player_movement(SDL_Rect *player) {
    if (player->y <= 0) {
        player->y = 0;
    }

    if (player->y + player->h >= SCREEN_HEIGHT) {
        player->y = SCREEN_HEIGHT - player->h;
    }
}

bool checkCollision(SDL_Rect *obj1, SDL_Rect *obj2) {
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = obj1->x;
    rightA = obj1->x + obj1->w;
    topA = obj1->y;
    bottomA = obj1->y + obj1->h;

    //Calculate the sides of rect B
    leftB = obj2->x;
    rightB = obj2->x + obj2->w;
    topB = obj2->y;
    bottomB = obj2->y + obj2->h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB ||
            topA >= bottomB ||
            rightA <= leftB ||
            leftA >= rightB) {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

int main(int argc, char* args[]) {
    //The window we'll be rendering to
    SDL_Window* window = NULL;

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

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

                if (key_states[SDL_SCANCODE_UP]) {
                    player1.y -= player_speed;
                }
                if (key_states[SDL_SCANCODE_DOWN]) {
                    player1.y += player_speed;
                }
                if (key_states[SDL_SCANCODE_W]) {
                    player2.y -= player_speed;
                }
                if (key_states[SDL_SCANCODE_S]) {
                    player2.y += player_speed;
                }
                if (key_states[SDL_SCANCODE_PAGEUP]) {
                    ball_speed += 2;
                }
                if (key_states[SDL_SCANCODE_PAGEDOWN]) {
                    ball_speed -= 2;
                }

                //Clear screen 
                SDL_SetRenderDrawColor(gRenderer, WHITE, 0xFF);
                SDL_RenderClear(gRenderer);

                player_movement(&player1);
                player_movement(&player2);

                //Draw Rect
                SDL_SetRenderDrawColor(gRenderer, BLACK, 0xFF);
                SDL_RenderDrawRect(gRenderer, &player1);
                SDL_RenderDrawRect(gRenderer, &player2);

                move_ball(&ball, &player1, &player2);
                SDL_RenderDrawRect(gRenderer, &(ball.coords));
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