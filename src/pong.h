#ifndef PONG_H
#define PONG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "GameEngine/Font/Font.h"

    void Pong_Init(const int start_x, const int start_y, const int window_height, const int window_width);
    void Pong_Handle(const Uint8 *);
    void Pong_SetRenderer(SDL_Renderer *);
    void Pong_Draw();
    void Pong_Destroy();

#ifdef __cplusplus
}
#endif

#endif /* PONG_H */

