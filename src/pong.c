#include "pong.h"
#include "fonts.h"
#include "pong_private.h"

void Pong_Init(const int start_x, const int start_y, const int window_height, const int window_width) {
    p_data = malloc(sizeof (struct Pong_Data));

    p_data->window = init_window(start_x, start_y, window_height, window_width);
    calculate_defaults(p_data);

    p_data->ball = init_ball();

    p_data->player1 = init_player1();
    p_data->player2 = init_player2();

    p_data->p1_score = 0;
    p_data->p2_score = 0;

    gf_p1_score = Game_Font_Init("./resources/font/lazy.ttf", 78);
    gf_p2_score = Game_Font_Init("./resources/font/lazy.ttf", 78);
}

void Pong_Handle(const Uint8 *keys) {
    handle_keys(p_data, keys);
    handle_players(p_data);
    enum Ball_State bstate = handle_ball(p_data);
    handle_scores(p_data, bstate);
}

void Pong_Draw(SDL_Renderer *gRenderer) {
    char score_p1[4] = {0};
    char score_p2[4] = {0};

    snprintf(score_p1, 4, "%d", p_data->p1_score);
    snprintf(score_p2, 4, "%d", p_data->p2_score);

    Game_Font_SetText(gf_p1_score, score_p1);
    Game_Font_SetText(gf_p2_score, score_p2);

    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
    SDL_RenderClear(gRenderer);

    //Draw Players
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(gRenderer, p_data->player1);
    SDL_RenderDrawRect(gRenderer, p_data->player2);

    //Draw "ball"
    SDL_RenderDrawRect(gRenderer, &(p_data->ball->coords));

    //Draw Text
    Game_Font_Write(gf_p1_score, gRenderer, &p1_score_dest);
    Game_Font_Write(gf_p2_score, gRenderer, &p2_score_dest);

    //Debug lines
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
    SDL_RenderDrawLine(gRenderer, center, 0, center, p_data->window->h);
    SDL_RenderDrawLine(gRenderer, center_half_left, 0, center_half_left, p_data->window->h);
    SDL_RenderDrawLine(gRenderer, center_half_right, 0, center_half_right, p_data->window->h);
    SDL_RenderDrawRect(gRenderer, &p1_score_dest);
    SDL_RenderDrawRect(gRenderer, &p2_score_dest);
}

void Pong_Destroy() {

}