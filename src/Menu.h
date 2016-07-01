#ifndef PONG_MENU_H
#define PONG_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GameEngine/Font/Font.h"
#include "GameEngine/dbg.h"

#ifdef __cplusplus
extern "C" {
#endif           

    typedef struct Menu_Container Menu_Container;
    typedef void (*Menu_ItemAction)(void *);
    Menu_Container *Menu_InitBase(SDL_Renderer *, void *[], void *);
    void Menu_Display(Menu_Container *, int, int);
    void Menu_Handle(Menu_Container *, const Uint8);

    //void Menu_ItemAdd(Menu_Container *, char *, Menu_ItemAction);
    //@todo Add Menu_Destroy

#define Menu_Init(a,b,c) Menu_InitBase(a, b, (void *)c);

#ifdef __cplusplus
}
#endif

#endif /* PONG_MENU_H */

