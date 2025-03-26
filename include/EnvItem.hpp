#ifndef ENV_ITEM_HPP
#define ENV_ITEM_HPP

#include "raylib.h"

struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
};

#endif