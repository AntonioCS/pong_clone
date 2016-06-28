
#ifndef FONTS_H
#define FONTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> //strcmp
#include "../dbg.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct GameEngine_Font_Data;

    struct GameEngine_Font {
        struct GameEngine_Font_Data *data;

        struct GameEngine_Font *(*setText)(struct GameEngine_Font *, char *);
        struct GameEngine_Font *(*setTextInt)(struct GameEngine_Font *, int);
        struct GameEngine_Font *(*setColour)(struct GameEngine_Font *, Uint8, Uint8, Uint8);
        struct GameEngine_Font *(*writeCentered)(struct GameEngine_Font *, int, int);
        struct GameEngine_Font *(*write)(struct GameEngine_Font *, SDL_Rect *);
        struct GameEngine_Font *(*setRenderer)(struct GameEngine_Font *, SDL_Renderer *);
        int (*getWidth)(struct GameEngine_Font *);
        int (*getHeight)(struct GameEngine_Font *);
    };

    struct GameEngine_Font *GameEngine_Font_Init(SDL_Renderer *, char *, int);

#define GameEngine_Font_Destroy(...) GameEngine_Font_DestroyBase((struct GameEngine_Font **[]){__VA_ARGS__, NULL});
    void GameEngine_Font_DestroyBase(struct GameEngine_Font ***);

#ifdef __cplusplus
}
#endif

#endif /* FONTS_H */

