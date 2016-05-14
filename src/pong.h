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

    void Pong_Init(const int window_height, const int window_width);
    void Pong_Handle(const Uint8 *);
    void Pong_Draw(SDL_Renderer *gRenderer);
    void Pong_Destroy();

#ifdef __cplusplus
}
#endif

#endif /* PONG_H */

