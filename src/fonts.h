/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fonts.h
 * Author: António Simões <antoniocs@gmail.com>
 *
 * Created on 12 May 2016, 18:50
 */

#ifndef FONTS_H
#define FONTS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct Game_Font;

    struct Game_Font *Game_Font_Init(char *fpath, int size);
    //void Game_Font_SetSize(struct Game_Font *gf, int size);
    void Game_Font_SetText(struct Game_Font *gf, char *);
    void Game_Font_SetColour(struct Game_Font *gf, Uint8 r, Uint8 g, Uint8 b);
    bool Game_Font_Write(struct Game_Font *gf, SDL_Renderer *r, SDL_Rect *dest);
    void Game_Font_Destroy(struct Game_Font *gf);

#ifdef __cplusplus
}
#endif

#endif /* FONTS_H */

