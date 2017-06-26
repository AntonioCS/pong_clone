#include "Font.h"
#include "Font_Private.h"

struct GameEngine_Font *GameEngine_Font_Init(SDL_Renderer *r, char *fpath, int size) {
    struct GameEngine_Font *self;

    check(TTF_WasInit(), "TTF has not been initialized");

    self = malloc(sizeof (*self));
    check_mem(self);

    struct GameEngine_Font_Data *data = malloc(sizeof (*data));
    check_mem(data);

    data->file_path = fpath;
    data->colour = default_colour();
    data->size = size;
    data->font = TTF_OpenFont(fpath, size);
    data->text = NULL;
    data->text_allocated = false;
    data->renderer = r;

    data->useLastTexture = false;
    data->lastTextureTxt = NULL;

    data->width = 0;
    data->height = 0;

    self->data = data;

    //Methods
    self->setText = setText;
    self->setTextInt = setTextInt;
    self->write = write;
    self->writeCentered = writeCentered;
    self->setColour = setColour;
    self->setRenderer = setRenderer;
    self->getHeight = getHeight;
    self->getWidth = getWidth;

    return self;

error:
    if (self) {
        free(self);
    }

    return NULL;
}

void GameEngine_Font_DestroyBase(struct GameEngine_Font ***list) {
    while (*list != NULL) {
        destroy(**list);
        **list = NULL;
        list++;
    }
}