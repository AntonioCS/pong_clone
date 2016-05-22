#include "fonts.h"

struct Game_Font {
    char *file_path;
    char *text;
    bool text_allocated;
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

static void check_for_allocated_text(struct Game_Font *gf) {
    if (gf->text_allocated && gf->text) {
        free(gf->text);

        gf->text = NULL;
        gf->text_allocated = false;
    }
}

struct Game_Font *Game_Font_Init(char *fpath, int size) {
    check(TTF_WasInit(), "TTF has not been initialized");

    struct Game_Font *gf = malloc(sizeof (struct Game_Font));
    check_mem(gf);

    gf->file_path = fpath;
    gf->colour = default_colour();
    gf->size = size;
    gf->font = TTF_OpenFont(fpath, size);
    gf->text = NULL;
    gf->text_allocated = false;

    return gf;

error:
    return NULL;
}

bool Game_Font_SetTextInt(struct Game_Font *gf, int num) {
    check_for_allocated_text(gf);

    //http://stackoverflow.com/a/8257728/8715 - Check in comments
    int length = snprintf(NULL, 0, "%d", num) + 1;
    char *text = calloc(length, sizeof (char));
    check_mem(text);

    snprintf(text, length, "%d", num);

    gf->text = text;
    gf->text_allocated = true;

    return true;
error:
    return false;
}

bool Game_Font_SetText(struct Game_Font *gf, char *text) {
    check_for_allocated_text(gf);
    gf->text = text;

    return true;
}

void Game_Font_SetColour(struct Game_Font *gf, Uint8 r, Uint8 g, Uint8 b) {
    gf->colour->r = r;
    gf->colour->g = g;
    gf->colour->b = b;
}

bool Game_Font_WriteCentered(struct Game_Font *gf, SDL_Renderer *r, int x, int y) {
    int w;
    int h;

    check(TTF_SizeText(gf->font, gf->text, &w, &h) == 0, "TTF error: %s", TTF_GetError());

    x -= w / 2;
    y -= h / 2;

    SDL_Rect coords = {
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };

    Game_Font_Write(gf, r, &coords);

    return true;

error:
    return false;
}

bool Game_Font_Write(struct Game_Font *gf, SDL_Renderer *r, SDL_Rect * dest) {
    check(gf->font, "Font has not been initialized");

    SDL_Surface *surfaceTxt = TTF_RenderText_Solid(gf->font, gf->text, *(gf->colour));
    check(surfaceTxt, "TTF_RenderText failed: %s", TTF_GetError());

    SDL_Texture *textureTxt = SDL_CreateTextureFromSurface(r, surfaceTxt);
    SDL_FreeSurface(surfaceTxt);
    check(textureTxt, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());

    int res = SDL_RenderCopy(r, textureTxt, NULL, dest);
    SDL_DestroyTexture(textureTxt);
    check(res == 0, "SDL_RenderCopy failed: %s", SDL_GetError());

    return true;

error:
    return false;
}

void Game_Font_Destroy(struct Game_Font * gf) {
    if (gf) {
        TTF_CloseFont(gf->font);
        gf->font = NULL;

        free(gf->colour);
        gf->colour = NULL;

        check_for_allocated_text(gf);

        free(gf);
    }
}