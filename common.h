#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

const int GS = 30;
const int COL = 10;
const int ROW = 20;
const int WIDTH = GS * COL;
const int HEIGHT = GS * ROW;

/// SDL_GetError()を表示してプログラム終了
void sdl_error(const std::string &msg);
/// TTF_GetError()を表示してプログラム終了
void ttf_error(const std::string &msg);

struct Texture {
    SDL_Texture *texture;
    int w, h;
};
