#include "pong.h"
#include "pong_private.h"

void Pong_Init(const int start_x, const int start_y, const int window_width, const int window_height) {
    p_data = malloc(sizeof (struct Pong_Data));

    p_data->window = init_window(start_x, start_y, window_height, window_width);
    calculate_defaults(p_data);

    p_data->ball = init_ball();

    p_data->player1 = init_player1();
    p_data->player2 = init_player2();

    p_data->p1_score = 0;
    p_data->p2_score = 0;

    char *pong_score_font_path = "./resources/font/press-start/prstart.ttf";

    p1Score = GameEngine_Font_Init(NULL, pong_score_font_path, default_font_score_size);
    p2Score = GameEngine_Font_Init(NULL, pong_score_font_path, default_font_score_size);
}

void Pong_Reset() {
    //backup data
    const int x = p_data->window->x;
    const int y = p_data->window->y;
    const int w = p_data->window->w;
    const int h = p_data->window->h;
    SDL_Renderer *r = p_data->r;

    Pong_Destroy();

    Pong_Init(x,y,w,h);
    Pong_SetRenderer(r);
}

void Pong_Handle(const Uint8 *keys) {
    handle_keys(p_data, keys);
    handle_players(p_data);
    enum Ball_State bstate = handle_ball(p_data);
    handle_scores(p_data, bstate);
}

void Pong_SetRenderer(SDL_Renderer *r) {
    p_data->r = r;

    p1Score->setRenderer(p1Score, r);
    p2Score->setRenderer(p2Score, r);
}

void Pong_Draw() {
    //Clear screen
    SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 0xFF);
    SDL_RenderClear(getRenderer());

    p1Score
            ->setTextInt(p1Score, getPlayer1Score())
            ->writeCentered(p1Score, center_half_left, default_font_margin_top_space)
            ;
    p2Score
            ->setTextInt(p2Score, getPlayer2Score())
            ->writeCentered(p2Score, center_half_right, default_font_margin_top_space)
            ;

    //Draw Players
    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 0xFF);
    SDL_RenderDrawRect(getRenderer(), p_data->player1);
    SDL_RenderDrawRect(getRenderer(), p_data->player2);

    //Draw "ball"
    SDL_RenderDrawRect(getRenderer(), &(p_data->ball->coords));

    //Debug lines
    /*
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0xFF);
    SDL_RenderDrawLine(gRenderer, center, 0, center, p_data->window->h);
    SDL_RenderDrawLine(gRenderer, center_half_left, 0, center_half_left, p_data->window->h);
    SDL_RenderDrawLine(gRenderer, center_half_right, 0, center_half_right, p_data->window->h);
    //*/
}

void Pong_Destroy() {
    free(p_data->window);
    p_data->window = NULL;

    free(p_data->ball);
    p_data->ball = NULL;

    free(p_data->player1);
    p_data->player1 = NULL;

    free(p_data->player2);
    p_data->player2 = NULL;

    GameEngine_Font_Destroy(&p1Score, &p2Score);

    free(p_data);
    p_data = NULL;
}