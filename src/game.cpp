#include "game.h"
#include <cassert>
#include <ctime>
#include <iostream>

void sdl_error(const std::string &msg) {
    std::cerr << msg << " SDL Error: " << SDL_GetError() << std::endl;
    exit(1);
}

void ttf_error(const std::string &msg) {
    std::cerr << msg << " SDL_ttf Error: " << TTF_GetError() << std::endl;
    exit(1);
}

SDL_Color get_color(TetriminoType t) {
    switch (t) {
    case TetriminoType::I:
        // 水色
        return SDL_Color{0, 255, 255, 255};
    case TetriminoType::O:
        // 黄色
        return SDL_Color{255, 255, 0, 255};
    case TetriminoType::S:
        // 緑
        return SDL_Color{0, 255, 0, 255};
    case TetriminoType::Z:
        // 赤
        return SDL_Color{255, 0, 0, 255};
    case TetriminoType::J:
        // 青
        return SDL_Color{0, 0, 255, 255};
    case TetriminoType::L:
        // オレンジ
        return SDL_Color{255, 128, 0, 255};
    case TetriminoType::T:
        // 紫
        return SDL_Color{128, 0, 128, 255};
    default:
        assert(false);
    }
}

Texture make_texture(SDL_Renderer *renderer, TTF_Font *font, const std::string &text,
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

Game::Game() : state(GameState::Start) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_error("SDLの初期化に失敗");
    if (TTF_Init() < 0)
        ttf_error("SDL_ttfの初期化に失敗");

    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                              height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        sdl_error("ウィンドウの初期化に失敗");

    renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        sdl_error("レンダラの初期化に失敗");

    font = TTF_OpenFont("data/roboto-android/Roboto-Black.ttf", 30);
    if (font == nullptr)
        ttf_error("TTF_OpenFontに失敗");

    srand(time(nullptr));
}

Game::~Game() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    SDL_Event e;
    bool quit = false;

    new_tetrimino();

    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_q:
                    quit = true;
                    break;
                }
            }
            update(e);
        }
        update();
        draw();

        SDL_RenderPresent(renderer);
    }
}

void Game::update() {
    switch (state) {
    case GameState::Play:
        update_play();
        break;
    default:
        break;
    }
}

void Game::update_play() {
    frame++;
    frame %= 24;
    if (frame == 0) {
        down();
    }
}

void Game::update(SDL_Event &e) {
    switch (state) {
    case GameState::Start:
        update_start(e);
        break;
    case GameState::Play:
        update_play(e);
        break;
    case GameState::Pause:
        update_pause(e);
        break;
    case GameState::GameOver:
        update_gameover(e);
        break;
    }
}

void Game::update_start(SDL_Event &e) {
    // 何らかのキーを押したらプレイ
    if (e.type == SDL_KEYDOWN)
        state = GameState::Play;
}

void Game::update_play(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            if (e.key.repeat == 0)
                rotate();
            break;
        case SDLK_SPACE:
            if (e.key.repeat == 0)
                drop();
            break;
        case SDLK_LEFT:
            move(-1, 0);
            break;
        case SDLK_RIGHT:
            move(1, 0);
            break;
        case SDLK_DOWN:
            move(0, 1);
            break;
        case SDLK_p:
        case SDLK_ESCAPE:
            state = GameState::Pause;
            break;
        }
    }
}

void Game::update_pause(SDL_Event &e) {
    // 何らかのキーを押したらプレイ
    if (e.type == SDL_KEYDOWN)
        state = GameState::Play;
}

void Game::update_gameover(SDL_Event &e) {
    // 何らかのキーを押したらreset
    if (e.type == SDL_KEYDOWN)
        reset();
}

void Game::draw() const {
    switch (state) {
    case GameState::Start:
        draw_start();
        break;
    case GameState::Play:
        draw_play();
        break;
    case GameState::Pause:
        draw_pause();
        break;
    case GameState::GameOver:
        draw_gameover();
        break;
    }
}

void Game::draw_start() const {
    Texture texture = make_texture(renderer, font, "Press any key to start", {0, 0, 255, 255});
    SDL_Rect rect = {(width - texture.w) / 2, (height - texture.h) / 2, texture.w, texture.h};
    SDL_RenderCopy(renderer, texture.texture, nullptr, &rect);
    SDL_DestroyTexture(texture.texture);
}

void Game::draw_play() const {
    draw_board();
    draw_current_shadow();
    draw_current();
}

void Game::draw_board() const {
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            auto field = board.board.at(y).at(x);
            if (field) {
                SDL_Rect rect = {x * gs, y * gs, gs, gs};
                auto [r, g, b, a] = get_color(field.value());
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void Game::draw_current() const {
    for (auto [x, y] : current.blocks()) {
        SDL_Rect rect = {x * gs, y * gs, gs, gs};
        auto [r, g, b, a] = get_color(current.type());
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::draw_current_shadow() const {
    auto [x, y] = current.pos();
    auto copy = current;
    int ly = board.limit_y(current);
    copy.set_pos(x, ly);
    for (auto [x, y] : copy.blocks()) {
        SDL_Rect rect = {x * gs, y * gs, gs, gs};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::draw_pause() const {
    draw_board();
    draw_current_shadow();
    draw_current();

    Texture texture = make_texture(renderer, font, "PAUSE", {255, 255, 0, 255});
    SDL_Rect rect = {(width - texture.w) / 2, (height - texture.h) / 2, texture.w, texture.h};
    SDL_RenderCopy(renderer, texture.texture, nullptr, &rect);
    SDL_DestroyTexture(texture.texture);
}

void Game::draw_gameover() const {
    draw_board();
    draw_current();

    Texture texture = make_texture(renderer, font, "GAME OVER", {255, 0, 0, 255});
    SDL_Rect rect = {(width - texture.w) / 2, (height - texture.h) / 2, texture.w, texture.h};
    SDL_RenderCopy(renderer, texture.texture, nullptr, &rect);
    SDL_DestroyTexture(texture.texture);
}

void Game::new_tetrimino() {
    board.delete_lines();
    TetriminoType type = static_cast<TetriminoType>(rand() % 7);
    current = Tetrimino(type);
    if (board.check_gameover(current)) {
        state = GameState::GameOver;
    }
}

void Game::set_tetrimino() {
    board.put(current);
    new_tetrimino();
}

void Game::down() {
    if (!move(0, 1))
        set_tetrimino();
}

void Game::drop() {
    board.drop(current);
    new_tetrimino();
}

bool Game::move(int x, int y) {
    auto copy = current;
    copy.move(x, y);
    if (!board.detect_collision(copy)) {
        current = copy;
        return true;
    }
    return false;
}

bool Game::rotate() {
    auto copy = current;
    copy.rotate();
    if (!board.detect_collision(copy)) {
        current = copy;
        return true;
    }
    return false;
}

void Game::reset() {
    board = Board();
    new_tetrimino();
    state = GameState::Start;
}
