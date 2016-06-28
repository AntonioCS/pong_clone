
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
        SDL_Texture *lastTextureTxt;
        bool useLastTexture;

        int width;
        int height;
    };

    static SDL_Colour *default_colour(void) {
        SDL_Colour *c = malloc(sizeof (SDL_Colour));

        c->r = 0;
        c->g = 0;
        c->b = 0;

        return c;
    }

    static void destroy_last_texture(struct GameEngine_Font_Data *data) {
        if (data->lastTextureTxt) {
            SDL_DestroyTexture(data->lastTextureTxt);
            data->lastTextureTxt = NULL;
        }
        data->useLastTexture = false;
    }

    static void destroy_allocated_text(struct GameEngine_Font_Data *data) {
        if (data->text_allocated && data->text) {
            free(data->text);
            data->text = NULL;
            data->text_allocated = false;
        }
    }

    static void check_for_previous_text_match(struct GameEngine_Font_Data *data, char *text) {
        if (data->text != NULL && data->lastTextureTxt != NULL && strcmp(data->text, text) == 0) {
            data->useLastTexture = true;
        } else {
            destroy_last_texture(data);
        }
    }

    static void calculate_text_widht_height(struct GameEngine_Font_Data *data) {
        TTF_SizeText(data->font, data->text, &(data->width), &(data->height));
    }

    static struct GameEngine_Font *setTextInt(struct GameEngine_Font *self, int num) {
        //http://stackoverflow.com/a/8257728/8715 - Check in comments
        int length = snprintf(NULL, 0, "%d", num) + 1;
        char *text = calloc(length, sizeof (char));
        check_mem(text);

        snprintf(text, length, "%d", num);

        check_for_previous_text_match(self->data, text);

        //NOTE: This needs to be called only after check_for_previous_text_match
        destroy_allocated_text(self->data);
        self->data->text = text;
        self->data->text_allocated = true;

        calculate_text_widht_height(self->data);

        return self;
error:
        return NULL;
    }

    static struct GameEngine_Font *setText(struct GameEngine_Font *self, char *text) {
        check_for_previous_text_match(self->data, text);
        destroy_allocated_text(self->data);

        self->data->text = text;

        calculate_text_widht_height(self->data);

        return self;
    }

    static struct GameEngine_Font *setColour(struct GameEngine_Font *self, Uint8 r, Uint8 g, Uint8 b) {

        if (self->data->colour->r != r || self->data->colour->g != g || self->data->colour->b != b) {
            destroy_last_texture(self->data);

            self->data->colour->r = r;
            self->data->colour->g = g;
            self->data->colour->b = b;
        }

        return self;
    }

    static struct GameEngine_Font *writeCentered(struct GameEngine_Font *self, int x, int y) {
        SDL_Rect coords = {
            .x = (x - self->data->width / 2),
            .y = (y - self->data->height / 2),
            .w = self->data->width,
            .h = self->data->height
        };

        return self->write(self, &coords);
    }

    static struct GameEngine_Font *write(struct GameEngine_Font *self, SDL_Rect *dest) {
        check(self->data->font, "Font has not been initialized");

        if (!self->data->useLastTexture || self->data->lastTextureTxt == NULL) {
            log_info("Creating new text texture");
            destroy_last_texture(self->data);

            SDL_Surface *surfaceTxt = TTF_RenderText_Solid(self->data->font, self->data->text, *(self->data->colour));
            check(surfaceTxt, "TTF_RenderText failed: %s", TTF_GetError());

            self->data->lastTextureTxt = SDL_CreateTextureFromSurface(self->data->renderer, surfaceTxt);
            self->data->useLastTexture = true;
            SDL_FreeSurface(surfaceTxt);
            check(self->data->lastTextureTxt, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        }

        int res = SDL_RenderCopy(self->data->renderer, self->data->lastTextureTxt, NULL, dest);
        check(res == 0, "SDL_RenderCopy failed: %s", SDL_GetError());

        return self;

error:
        destroy_last_texture(self->data);
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

            destroy_allocated_text(self->data);
            destroy_last_texture(self->data);

            free(self->data);
            self->data = NULL;

            free(self);
        }
    }

    static int getWidth(struct GameEngine_Font *self) {
        return self->data->width;
    }

    static int getHeight(struct GameEngine_Font *self) {
        return self->data->height;
    }

#ifdef __cplusplus
}
#endif

#endif /* FONT_PRIVATE_H */

