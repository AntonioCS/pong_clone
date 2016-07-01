#include "Menu.h"

typedef struct {
    char *name;
    void (*action)(void *);
    void *data;
    struct GameEngine_Font *text;
    bool selected;
} Menu_Item;

struct Menu_Container {
    unsigned int total;
    void *data;
    SDL_Renderer *r;
    Menu_Item **items;
};

static int count_items(void **items) {
    int count = 0;

    while (*items++) {
        count++;
    }

    return count;
}

static char *font_path = "./resources/font/press-start/prstart.ttf";

static Menu_Item **create_menu_items(SDL_Renderer *r, void **menu_items, int total) {
    Menu_Item **items = malloc(sizeof (Menu_Item *) * (total / 2));

    for (int i = 0, t = 0; i < total; t++, i += 2) {
        items[t] = malloc(sizeof (Menu_Item));

        items[t]->name = menu_items[i];
        items[t]->action = menu_items[i + 1];
        items[t]->data = NULL;

        struct GameEngine_Font *f = GameEngine_Font_Init(r, font_path, 20);
        f->setColour(f, 0, 0, 0);
        f->setText(f, menu_items[i]);

        items[t]->text = f;       
        items[t]->selected = (t == 0); //set only the first as true
        
    }

    return items;
}

Menu_Container *Menu_InitBase(SDL_Renderer *r, void *items[], void *data) {
    Menu_Container *mc = malloc(sizeof (Menu_Container));
    int total = count_items(items);

    mc->items = create_menu_items(r, items, total);
    mc->total = total / 2;
    mc->data = data;
    mc->r = r;

    return mc;
}

static void set_selected(Menu_Container *mc, bool go_up) {
    int sel = 0;
    int i = 0;
    int last = mc->total - 1;

    for (; i < mc->total; i++) {
        if (mc->items[i]->selected) {
            if (go_up) {
                sel = (i == 0) ? last : (i - 1);                
            } else {
                sel = (i == last) ? 0 : (i + 1);                
            }            
            break;
        }
    }
    
    log_info("Sel: %d", sel);
    log_info("i: %d", i);

    mc->items[sel]->selected = true;
    mc->items[i]->selected = false;
}

static void trigger_selected(Menu_Container *mc) {
    for (int i = 0; i < mc->total; i++) {
        if (mc->items[i]->selected) {
            Menu_ItemAction action = mc->items[i]->action;
            action(mc->data);
            
            break;
        }
    }
}

void Menu_Handle(Menu_Container *mc, const Uint8 keys) {

    switch (keys) {
        case SDL_SCANCODE_UP:
            log_info("Pressed up");
            set_selected(mc, true);
            break;
        case SDL_SCANCODE_DOWN:
            log_info("Pressed down");
            set_selected(mc, false);
            break;
        case SDLK_RETURN:
        case SDL_SCANCODE_KP_ENTER:            
            log_info("Pressed enter");
            trigger_selected(mc);
            break;
    }
}

void Menu_Display(Menu_Container *mc, int x, int y) {
    struct GameEngine_Font *f;
    int prevHeight = 0;

    for (int i = 0; i < mc->total; i++) {
        f = mc->items[i]->text;

        if (mc->items[i]->selected) {
            //log_info("Selected %d", i);
            f->setColour(f, 255, 0, 0);
        } else {
            f->setColour(f, 0, 0, 0);
        }

        f->writeCentered(f, x, y + prevHeight);

        prevHeight += f->getHeight(f) + 10;
    }
}


//@todo Finish this in case I need to add menus dynamically
void Menu_ItemAdd(Menu_Container *mc, char *name, Menu_ItemAction action) {
    if (mc->items == NULL) {

    }
}


