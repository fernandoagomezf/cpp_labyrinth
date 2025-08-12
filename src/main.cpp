
#include "common.h"

using std::cout;
using std::endl;

int SDL_main(int argc, char* argv[]) {
    std::cout << "Hello SDL2!" << std::endl;
    
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Your SDL code here...

    SDL_Quit();
    return 0;
}
