#include "common.h"

int SDL_main(int argc, char* argv[]) {
    std::cout << "SDL2 Initializing..." << std::endl;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    return 0;
}