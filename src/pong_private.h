#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "GameEngine/Font/Font.h"
#include "GameEngine/dbg.h"

#ifndef PONG_PRIVATE_H
#define PONG_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif  

    enum Ball_State {
        ball_state_moving,
        ball_state_hit_wall_left,
        ball_state_hit_wall_right,
        ball_state_hit_player,
        ball_state_total_states
    };

    struct Pong_Ball {
        SDL_Rect coords;
        unsigned int left;
        unsigned int up;
    };

    struct Pong_Window {
        const int x;
        const int y;
        const int h;
        const int w;
    };

    struct Pong_Data {
        struct Pong_Ball *ball;
        struct Pong_Window *window;

        SDL_Rect *player1;
        SDL_Rect *player2;

        unsigned int p1_score;
        unsigned int p2_score;

        SDL_Renderer *r;
    };

    struct Pong_Data *p_data;

    struct GameEngine_Font *p1Score;
    struct GameEngine_Font *p2Score;

    static int ball_speed = 2;
    static int player_speed = 10;

    //<editor-fold defaultstate="collapsed" desc="Default values">
    static int default_margin_side_space = 20;
    static int default_margin_top_space = 30;
    static int default_font_margin_top_space = 80;

    static int default_ball_percentage = 4;
    static int default_ball_x;
    static int default_ball_y;
    static int default_ball_w;
    static int default_ball_h;

    static int default_player1_x;
    static int default_player1_y;
    static int default_player1_w = 20;
    static int default_player1_h = 100;

    static int default_player2_x;
    static int default_player2_y;
    static int default_player2_w = 20;
    static int default_player2_h = 100;

    static int default_paddles_percentage = 25;

    static int default_font_score_size_percentage = 18;
    static int default_font_score_size;
    //</editor-fold>

    static int center;
    static int center_half_left;
    static int center_half_right;

    static struct Pong_Window *init_window(const int window_x, const int window_y, const int window_height, const int window_width);
    static struct Pong_Ball *init_ball();
    static SDL_Rect *init_player1();
    static SDL_Rect *init_player2();
    static void handle_keys(struct Pong_Data *pd, const Uint8 *k);
    static enum Ball_State handle_ball(struct Pong_Data *pd);
    static void handle_players(struct Pong_Data *pd);
    static void player_movement(SDL_Rect *player, const int height);
    static bool check_for_collision(SDL_Rect *obj1, SDL_Rect *obj2);
    static void handle_scores(struct Pong_Data *pd, enum Ball_State);
    static void calculate_defaults(struct Pong_Data *pd);
    static int inline get_percentage_value(int value, int percentage);
    static int getPlayer1Score();
    static int getPlayer2Score();
    static SDL_Renderer *getRenderer();

    struct Pong_Window *init_window(const int window_x, const int window_y, const int window_height, const int window_width) {
        //http://stackoverflow.com/questions/9691404/how-to-initialize-const-in-a-struct-in-c-with-malloc
        struct Pong_Window p_window = {
            .x = window_x,
            .y = window_y,
            .h = window_height,
            .w = window_width
        };

        struct Pong_Window *pw = malloc(sizeof (struct Pong_Window));
        memcpy(pw, &p_window, sizeof (struct Pong_Window));

        return pw;
    }

    struct Pong_Ball *init_ball() {
        struct Pong_Ball ball = {
            .coords.x = default_ball_x,
            .coords.y = default_ball_y,
            .coords.w = default_ball_w,
            .coords.h = default_ball_h,
            .left = 0,
            .up = 0
        };

        struct Pong_Ball *pb = malloc(sizeof (struct Pong_Ball));
        check_mem(pb);
        memcpy(pb, &ball, sizeof (struct Pong_Ball));

        return pb;
error:
        return NULL;
    }

    SDL_Rect *init_player1() {
        SDL_Rect *p = malloc(sizeof (SDL_Rect));
        check_mem(p);

        p->x = default_player1_x;
        p->y = default_player1_y;
        p->w = default_player1_w;
        p->h = default_player1_h;

        return p;
error:
        return NULL;
    }

    SDL_Rect *init_player2() {
        SDL_Rect *p = malloc(sizeof (SDL_Rect));
        check_mem(p);

        p->x = default_player2_x;
        p->y = default_player2_y;
        p->w = default_player2_w;
        p->h = default_player2_h;

        return p;
error:
        return NULL;
    }

    void handle_keys(struct Pong_Data *pd, const Uint8 * key_states) {
        if (key_states[SDL_SCANCODE_UP]) {
            pd->player1->y -= player_speed;
        }
        if (key_states[SDL_SCANCODE_DOWN]) {
            pd->player1->y += player_speed;
        }
        if (key_states[SDL_SCANCODE_W]) {
            pd->player2->y -= player_speed;
        }
        if (key_states[SDL_SCANCODE_S]) {
            pd->player2->y += player_speed;
        }
        if (key_states[SDL_SCANCODE_PAGEUP]) {
            ball_speed += 2;
        }
        if (key_states[SDL_SCANCODE_PAGEDOWN]) {
            ball_speed -= 2;
        }
    }

    enum Ball_State handle_ball(struct Pong_Data *pd) {
        int speed = ball_speed;
        enum Ball_State bstate = ball_state_moving;

        //moving to the right
        if (pd->ball->left == 0) {
            if (!check_for_collision(&(pd->ball->coords), pd->player2)) {
                pd->ball->coords.x += speed;
            } else {
                pd->ball->left = 1;
                bstate = ball_state_hit_player;
            }
        } else {
            //moving to the left
            if (!check_for_collision(&(pd->ball->coords), pd->player1)) {
                pd->ball->coords.x -= speed;
            } else {
                pd->ball->left = 0;
                bstate = ball_state_hit_player;
            }
        }

        if (pd->ball->up == 0) {
            pd->ball->coords.y += speed;
        } else {
            pd->ball->coords.y -= speed;
        }

        //Hit the wall!!!
        if (pd->ball->coords.x <= 0) {
            pd->ball->left = 0;
            bstate = ball_state_hit_wall_left;
        } else if (pd->ball->coords.x + pd->ball->coords.w >= pd->window->w) {
            pd->ball->left = 1;
            bstate = ball_state_hit_wall_right;
        }

        if (pd->ball->coords.y <= 0) {
            pd->ball->up = 0;
        } else if (pd->ball->coords.y + pd->ball->coords.h >= pd->window->h) {
            pd->ball->up = 1;
        }

        return bstate;
    }

    void handle_players(struct Pong_Data *pd) {
        player_movement(pd->player1, pd->window->h);
        player_movement(pd->player2, pd->window->h);
    }

    void player_movement(SDL_Rect *player, const int height) {
        if (player->y <= 0) {
            player->y = 0;
        }

        if (player->y + player->h >= height) {
            player->y = height - player->h;
        }
    }

    bool check_for_collision(SDL_Rect *obj1, SDL_Rect *obj2) {
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

    void handle_scores(struct Pong_Data *pd, enum Ball_State bstate) {
        switch (bstate) {
            case ball_state_hit_wall_right:
                pd->p1_score++;
                break;
            case ball_state_hit_wall_left:
                pd->p2_score++;
                break;
            default:
                ;
        }
    }

    void calculate_defaults(struct Pong_Data *pd) {
        if (pd->window) {
            default_font_score_size = get_percentage_value(pd->window->h, default_font_score_size_percentage);
            int paddles_h = get_percentage_value(pd->window->h, default_paddles_percentage);

            default_player1_h = paddles_h;
            default_player2_h = paddles_h;

            //left side player
            default_player1_x = pd->window->x + default_margin_side_space;
            default_player1_y = pd->window->y + default_margin_top_space;

            //right side player
            default_player2_x = pd->window->w - default_margin_side_space - default_player2_w;
            default_player2_y = default_player1_y;

            //centers
            center = pd->window->w / 2;
            center_half_left = pd->window->w / 4;
            center_half_right = center + center_half_left;

            //ball
            default_ball_x = 120;
            default_ball_y = 30;

            default_ball_w = default_ball_h = get_percentage_value(pd->window->w, default_ball_percentage);
        }
    }

    int get_percentage_value(int value, int percentage) {
        return (float) value * (float) percentage / 100;
    }

    int getPlayer1Score() {
        return p_data->p1_score;
    }

    int getPlayer2Score() {
        return p_data->p2_score;
    }

    static SDL_Renderer *getRenderer() {
        return p_data->r;
    }

#ifdef __cplusplus
}
#endif

#endif /* PONG_PRIVATE_H */

