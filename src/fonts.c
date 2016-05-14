#include "fonts.h"

struct Game_Font {
    char *file_path;
    char *text;
    SDL_Color *colour;
    TTF_Font *font;
    unsigned int size;
};

static SDL_Colour *default_colour() {
    SDL_Colour *c = malloc(sizeof (SDL_Colour));

    c->r = 0;
    c->g = 0;
    c->b = 0;

    return c;
}

struct Game_Font *Game_Font_Init(char *fpath, int size) {
    if (TTF_WasInit() == 0) {
        goto ttf_error;
    }

    struct Game_Font *gf = malloc(sizeof (struct Game_Font));

    if (gf == NULL) {
        goto mem_error;
    }

    gf->file_path = fpath;
    gf->colour = default_colour();
    gf->size = size;
    gf->font = TTF_OpenFont(fpath, size);
    gf->text = NULL;

    return gf;

ttf_error:
    fprintf(stderr, "TTF has not been initialized");
    return NULL;
mem_error:
    perror("Error while allocating memory");
    return NULL;
}

/*void Game_Font_SetSize(struct Game_Font *gf, int size);*/
void Game_Font_SetText(struct Game_Font *gf, char *text) {
    gf->text = text;
}

void Game_Font_SetColour(struct Game_Font *gf, Uint8 r, Uint8 g, Uint8 b) {
    gf->colour->r = r;
    gf->colour->g = g;
    gf->colour->b = b;
}

bool Game_Font_Write(struct Game_Font *gf, SDL_Renderer *r, SDL_Rect *dest) {
    if (gf->font == NULL) {
        fprintf(stderr, "Font has not been initialized\n");
        goto error;
    }

    SDL_Surface *surfaceTxt = TTF_RenderText_Solid(gf->font, gf->text, *(gf->colour));

    if (surfaceTxt == NULL) {
        fprintf(stderr, "TTF_RenderText failed: %s\n", TTF_GetError());
        goto error;
    }

    SDL_Texture *textureTxt = SDL_CreateTextureFromSurface(r, surfaceTxt);
    SDL_FreeSurface(surfaceTxt);

    if (textureTxt == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        goto error;
    }

    int res = SDL_RenderCopy(r, textureTxt, NULL, dest);
    SDL_DestroyTexture(textureTxt);

    if (res < 0) {
        fprintf(stderr, "SDL_RenderCopy failed: %s\n", SDL_GetError());
        goto error;
    }

    return true;

error:
    return false;
}

void Game_Font_Destroy(struct Game_Font *gf) {
    if (gf) {
        TTF_CloseFont(gf->font);
        gf->font = NULL;

        free(gf->colour);
        gf->colour = NULL;

        free(gf);
    }
}