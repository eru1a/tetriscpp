#include "menu.h"

void Menu::draw(SDL_Renderer *renderer, TTF_Font *font) const {
    draw_texture(renderer, font, title, {std::nullopt, 100}, {255, 50, 50, 255});
    for (size_t i = 0; i < items.size(); i++) {
        int top = 200 + big_font_height * i + margin * i;
        std::pair<std::optional<int>, std::optional<int>> topleft = {std::nullopt, top};
        if (int(i) == select_index)
            draw_texture(renderer, font, items.at(i).first, topleft, SDL_Color{50, 50, 255, 255},
                         SDL_Color{150, 150, 150, 255});
        else
            draw_texture(renderer, font, items.at(i).first, topleft, SDL_Color{50, 50, 255, 255});
    }
}

void Menu::update(SDL_Event &e) {
    if (e.key.type != SDL_KEYDOWN)
        return;
    switch (e.key.keysym.sym) {
    case SDLK_DOWN:
    case SDLK_j:
        select_index++;
        if (select_index >= int(items.size()))
            select_index = items.size() - 1;
        break;
    case SDLK_UP:
    case SDLK_k:
        select_index--;
        if (select_index <= 0)
            select_index = 0;
        break;
    case SDLK_RETURN:
    case SDLK_SPACE:
        items.at(select_index).second();
        break;
    }
}
