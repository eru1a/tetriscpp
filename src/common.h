#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <optional>

const int gs = 30;
const int col = 10;
const int row = 20;
const int board_width = gs * col;
const int board_height = gs * row;
const int next_area_width = gs * 4;
const int next_area_height = gs * 2;
const int margin = 10;
const int font_height = 36;
const int big_font_height = 71;
const int width = board_width + margin + next_area_width + margin;
const int height = board_height;

inline void sdl_error(const std::string &msg) {
    std::cerr << msg << " SDL Error: " << SDL_GetError() << std::endl;
    exit(1);
}

inline void ttf_error(const std::string &msg) {
    std::cerr << msg << " SDL_ttf Error: " << TTF_GetError() << std::endl;
    exit(1);
}

struct Texture {
    SDL_Texture *texture;
    int w, h;
};

inline Texture make_texture(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                            SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr)
        ttf_error("start_surfaceの作成に失敗");
    Texture texture = {SDL_CreateTextureFromSurface(renderer, surface), surface->w, surface->h};
    if (texture.texture == nullptr)
        sdl_error("textureの作成に失敗");
    SDL_FreeSurface(surface);
    return texture;
}

// textからTextureを生成して描画
// topleftの座標がnoneだった場合中央に描画
// backgroundが渡されていたら背景をRectで描画
inline void draw_texture(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
                         const std::pair<std::optional<int>, std::optional<int>> &topleft,
                         const SDL_Color &color,
                         std::optional<SDL_Color> background = std::nullopt) {
    Texture texture = make_texture(renderer, font, text, color);
    auto [l, t] = topleft;
    int left = l.has_value() ? l.value() : ((width - texture.w) / 2);
    int top = t.has_value() ? t.value() : ((height - texture.h) / 2);
    SDL_Rect rect{left, top, texture.w, texture.h};
    if (background.has_value()) {
        auto [r, g, b, a] = background.value();
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderCopy(renderer, texture.texture, nullptr, &rect);
    SDL_DestroyTexture(texture.texture);
}
