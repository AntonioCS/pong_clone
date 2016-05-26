
#ifndef FONT_PRIVATE_H
#define FONT_PRIVATE_H
#include "Font.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct GameEngine_Font_Data {
        char *file_path;

        char *text;
        bool text_allocated;

        SDL_Color *colour;
        TTF_Font *font;
        unsigned int size;

        SDL_Renderer *renderer;
    };

    static SDL_Colour *default_colour() {
        SDL_Colour *c = malloc(sizeof (SDL_Colour));

        c->r = 0;
        c->g = 0;
        c->b = 0;

        return c;
    }

    static void check_for_allocated_text(struct GameEngine_Font_Data *data) {
        if (data->text_allocated && data->text) {
            free(data->text);

            data->text = NULL;
            data->text_allocated = false;
        }
    }

    static struct GameEngine_Font *setTextInt(struct GameEngine_Font *self, int num) {
        check_for_allocated_text(self->data);

        //http://stackoverflow.com/a/8257728/8715 - Check in comments
        int length = snprintf(NULL, 0, "%d", num) + 1;
        char *text = calloc(length, sizeof (char));
        check_mem(text);

        snprintf(text, length, "%d", num);

        self->data->text = text;
        self->data->text_allocated = true;

        return self;
error:
        return NULL;
    }

    static struct GameEngine_Font *setText(struct GameEngine_Font *self, char *text) {
        check_for_allocated_text(self->data);
        self->data->text = text;

        return self;
    }

    static struct GameEngine_Font *setColour(struct GameEngine_Font *self, Uint8 r, Uint8 g, Uint8 b) {
        self->data->colour->r = r;
        self->data->colour->g = g;
        self->data->colour->b = b;

        return self;
    }

    static struct GameEngine_Font *writeCentered(struct GameEngine_Font *self, int x, int y) {
        int w;
        int h;

        check(TTF_SizeText(self->data->font, self->data->text, &w, &h) == 0, "TTF error: %s", TTF_GetError());

        x -= w / 2;
        y -= h / 2;

        SDL_Rect coords = {
            .x = x,
            .y = y,
            .w = w,
            .h = h
        };

        return self->write(self, &coords);

error:
        return NULL;
    }

    static struct GameEngine_Font *write(struct GameEngine_Font *self, SDL_Rect *dest) {
        check(self->data->font, "Font has not been initialized");

        SDL_Surface *surfaceTxt = TTF_RenderText_Solid(self->data->font, self->data->text, *(self->data->colour));
        check(surfaceTxt, "TTF_RenderText failed: %s", TTF_GetError());

        SDL_Texture *textureTxt = SDL_CreateTextureFromSurface(self->data->renderer, surfaceTxt);
        SDL_FreeSurface(surfaceTxt);
        check(textureTxt, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());

        int res = SDL_RenderCopy(self->data->renderer, textureTxt, NULL, dest);
        SDL_DestroyTexture(textureTxt);
        check(res == 0, "SDL_RenderCopy failed: %s", SDL_GetError());

        return self;

error:
        return NULL;
    }

    static struct GameEngine_Font *setRenderer(struct GameEngine_Font *self, SDL_Renderer *r) {
        self->data->renderer = r;
        return self;
    }

    static void destroy(struct GameEngine_Font *self) {
        if (self) {
            TTF_CloseFont(self->data->font);
            self->data->font = NULL;

            free(self->data->colour);
            self->data->colour = NULL;

            check_for_allocated_text(self->data);

            free(self->data);
            self->data = NULL;

            free(self);
        }
    }


#ifdef __cplusplus
}
#endif

#endif /* FONT_PRIVATE_H */

