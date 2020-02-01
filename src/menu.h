#pragma once

#include "common.h"
#include <functional>
#include <string>
#include <vector>

using MenuItem = std::pair<std::string, std::function<void(void)>>;

class Menu {
public:
    Menu(const std::string &title, const std::vector<MenuItem> &items)
        : title(title), items(items) {}
    void draw(SDL_Renderer *renderer, TTF_Font *font) const;
    void update(SDL_Event &e);

private:
    std::string title;
    std::vector<MenuItem> items;
    int select_index = 0;
};
