/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pong_private.h
 * Author: António Simões <antoniocs@gmail.com>
 *
 * Created on 08 May 2016, 08:20
 */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> //sleep

#ifndef PONG_PRIVATE_H
#define PONG_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif  

    static int ball_speed = 2;
    static int player_speed = 10;

    static int default_ball_x = 120;
    static int default_ball_y = 30;
    static int default_ball_w = 20;
    static int default_ball_h = 20;

    static int default_player1_x = 20;
    static int default_player1_y = 30;
    static int default_player1_w = 20;
    static int default_player1_h = 100;

    static int default_player2_x = 600;
    static int default_player2_y = 30;
    static int default_player2_w = 20;
    static int default_player2_h = 100;

    struct Pong_Ball {
        SDL_Rect coords;
        unsigned int left;
        unsigned int up;
    };

    struct Pong_Window {
        const int height;
        const int width;
    };

    struct Pong_Data {
        struct Pong_Ball *ball;
        struct Pong_Window *window;
        SDL_Rect *player1;
        SDL_Rect *player2;
    };

    static struct Pong_Window *init_window(const int window_height, const int window_width);
    static struct Pong_Ball *init_ball();
    static SDL_Rect *init_player1();
    static SDL_Rect *init_player2();
    static void handle_keys(struct Pong_Data *pd, const Uint8 *k);
    static void handle_ball(struct Pong_Data *pd);
    static void handle_players(struct Pong_Data *pd);
    static void player_movement(SDL_Rect *player, const int height);
    static bool check_for_collision(SDL_Rect *obj1, SDL_Rect *obj2);
    static void debug_print_rect(SDL_Rect *r);

    struct Pong_Window * init_window(const int window_height, const int window_width) {
        //http://stackoverflow.com/questions/9691404/how-to-initialize-const-in-a-struct-in-c-with-malloc
        struct Pong_Window p_window = {
            .height = window_height,
            .width = window_width
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
        memcpy(pb, &ball, sizeof (struct Pong_Ball));

        return pb;
    }

    SDL_Rect *init_player1() {
        SDL_Rect *p = malloc(sizeof (SDL_Rect));

        p->x = default_player1_x;
        p->y = default_player1_y;
        p->w = default_player1_w;
        p->h = default_player1_h;

        return p;
    }

    SDL_Rect *init_player2() {
        SDL_Rect *p = malloc(sizeof (SDL_Rect));

        p->x = default_player2_x;
        p->y = default_player2_y;
        p->w = default_player2_w;
        p->h = default_player2_h;

        return p;
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

    void handle_ball(struct Pong_Data *pd) {
        int speed = ball_speed;

        //moving to the right
        if (pd->ball->left == 0) {
            if (!check_for_collision(&(pd->ball->coords), pd->player2)) {
                pd->ball->coords.x += speed;
            } else {
                debug_print_rect(&(pd->ball->coords));
                debug_print_rect(pd->player2);
                printf("Entrei aqui1111111!\n");
                pd->ball->left = 1;
                sleep(10);
            }
        } else {
            //moving to the left
            if (!check_for_collision(&(pd->ball->coords), pd->player1)) {
                pd->ball->coords.x -= speed;
            } else {
                pd->ball->left = 0;
            }
        }

        if (pd->ball->up == 0) {
            pd->ball->coords.y += speed;
        } else {
            pd->ball->coords.y -= speed;
        }

        if (pd->ball->coords.x <= 0) {
            pd->ball->left = 0;
        } else if (pd->ball->coords.x + pd->ball->coords.w >= pd->window->width) {

            pd->ball->left = 1;
        }

        if (pd->ball->coords.y <= 0) {
            pd->ball->up = 0;
        } else if (pd->ball->coords.y + pd->ball->coords.h >= pd->window->height) {
            pd->ball->up = 1;
        }
    }

    void handle_players(struct Pong_Data *pd) {
        player_movement(pd->player1, pd->window->height);
        player_movement(pd->player2, pd->window->height);
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

    void debug_print_rect(SDL_Rect *r) {
        printf("x: %d, y: %d, w: %d, h: %d\n", r->x, r->y, r->w, r->h);
    }

#ifdef __cplusplus
}
#endif

#endif /* PONG_PRIVATE_H */

