
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dbg.h"

#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
extern "C" {
#endif

    struct Game_Font;

    struct Game_Font *Game_Font_Init(char *fpath, int size);
    //void Game_Font_SetSize(struct Game_Font *gf, int size);
    bool Game_Font_SetText(struct Game_Font *gf, char *);
    bool Game_Font_SetTextInt(struct Game_Font *gf, int num);
    void Game_Font_SetColour(struct Game_Font *gf, Uint8 r, Uint8 g, Uint8 b);
    bool Game_Font_WriteCentered(struct Game_Font *gf, SDL_Renderer *r, int x, int y);
    bool Game_Font_Write(struct Game_Font *gf, SDL_Renderer *r, SDL_Rect *dest);
    void Game_Font_Destroy(struct Game_Font *gf);

#ifdef __cplusplus
}
#endif

#endif /* FONTS_H */

