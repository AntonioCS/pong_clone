#include "pong.h"
#include "pong_private.h"

struct Pong_Data *p_data;

static SDL_Color color;
static TTF_Font *font;
static SDL_Surface *font_text;
static SDL_Texture *mTexture;

void Pong_Init(const int window_height, const int window_width) {
    p_data = malloc(sizeof (struct Pong_Data));

    p_data->window = init_window(window_height, window_width);
    p_data->ball = init_ball();

    p_data->player1 = init_player1();
    p_data->player2 = init_player2();

    p_data->p1_score = 0;
    p_data->p2_score = 0;

    font = TTF_OpenFont("./resources/font/lazy.ttf", 78);
    color.r = 0;
    color.g = 0;
    color.b = 0;

    if (font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    font_text = TTF_RenderText_Blended(font, "01", color);
    //font_text = TTF_RenderText_Solid(font, "01", color);
}

void Pong_Handle(const Uint8 *keys) {
    handle_keys(p_data, keys);
    handle_players(p_data);
    enum Ball_State bstate = handle_ball(p_data);
    handle_scores(p_data, bstate);
}

void Pong_Draw(SDL_Renderer *gRenderer) {
    SDL_Rect test_struct = {
        .x = 120,
        .y = 20,
        .w = 150,
        .h = 120
    };
    mTexture = SDL_CreateTextureFromSurface( gRenderer, font_text);

    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
    SDL_RenderClear(gRenderer);

    //Draw Players
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(gRenderer, p_data->player1);
    SDL_RenderDrawRect(gRenderer, p_data->player2);

    //Draw "ball"
    SDL_RenderDrawRect(gRenderer, &(p_data->ball->coords));

    //Draw TExt
    SDL_RenderCopy(gRenderer, mTexture, NULL, &test_struct);
}