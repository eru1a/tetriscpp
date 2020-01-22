#include "common.h"

void sdl_error(const std::string &msg) {
    std::cerr << msg << " SDL Error: " << SDL_GetError() << std::endl;
    exit(1);
}

void ttf_error(const std::string &msg) {
    std::cerr << msg << " SDL_ttf Error: " << TTF_GetError() << std::endl;
    exit(1);
}
