#pragma once

#include "common.h"

namespace game {
    enum CellType { WALL, PATH, PLAYER, EXIT };

    struct Cell {
        CellType type;
        SDL_Texture* texture;
        SDL_Rect rect;
    };
}