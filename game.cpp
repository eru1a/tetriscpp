#include "game.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

Texture Game::make_texture(const std::string &text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
    if (surface == nullptr)
        ttf_error("start_surfaceの作成に失敗");
    Texture texture = {SDL_CreateTextureFromSurface(m_renderer, surface), surface->w, surface->h};
    if (texture.texture == nullptr)
        sdl_error("textureの作成に失敗");
    SDL_FreeSurface(surface);
    return texture;
}

Game::Game() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_error("SDLの初期化に失敗");
    if (TTF_Init() < 0)
        ttf_error("SDL_ttfの初期化に失敗");

    m_window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
                                HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
        sdl_error("ウィンドウの初期化に失敗");

    m_renderer =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
        sdl_error("レンダラの初期化に失敗");

    m_font = TTF_OpenFont("data/roboto-android/Roboto-Black.ttf", 30);
    if (m_font == nullptr)
        ttf_error("TTF_OpenFontに失敗");

    m_state = State::Start;

    m_start_texture = make_texture("Press any key to start", {0, 0, 255, 255});
    m_pause_texture = make_texture("Pause", {0, 0, 255, 255});
    m_gameover_texture = make_texture("GAME OVER", {255, 0, 0, 255});

    srand(time(nullptr));
}

Game::~Game() {
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_start_texture.texture);
    SDL_DestroyTexture(m_pause_texture.texture);
    SDL_DestroyTexture(m_gameover_texture.texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    SDL_Event e;
    bool quit = false;

    new_tetrimino();

    while (!quit) {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);

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

        SDL_RenderPresent(m_renderer);
    }
}

bool Game::check_gameover() const {
    if (!m_current)
        return false;
    auto tetrimino = m_current.value();
    auto blocks = tetrimino.get_blocks();
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!blocks.at(y).at(x))
                continue;
            int nx = start_x + x;
            int ny = start_y + y;
            if (m_board.at(ny).at(nx)) {
                return true;
            }
        }
    }
    return false;
}

void Game::new_tetrimino() {
    TetriminoType type = static_cast<TetriminoType>(rand() % 7);
    m_current = Tetrimino{start_x, start_y, 0, type};
}

void Game::put() {
    set_tetrimino();
    delete_lines();
    new_tetrimino();
    if (check_gameover())
        m_state = State::GameOver;
}

void Game::down() {
    if (!move(0, 1)) {
        put();
    }
}

int Game::limit_y() const {
    assert(m_current);
    auto copy = m_current.value();
    auto blocks = copy.get_blocks();
    while (check_collision(copy)) {
        copy.y++;
    }
    return copy.y - 1;
}

void Game::drop() {
    assert(m_current);
    m_current.value().y = limit_y();
    put();
}

void Game::set_tetrimino() {
    assert(m_current);
    auto tetrimino = m_current.value();
    auto blocks = tetrimino.get_blocks();
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!blocks.at(y).at(x))
                continue;
            int nx = tetrimino.x + x;
            int ny = tetrimino.y + y;
            m_board.at(ny).at(nx) = tetrimino.type;
        }
    }
}

bool Game::rotate() {
    auto copy = m_current.value();
    copy.rotate();
    if (check_collision(copy)) {
        m_current.value() = copy;
        return true;
    }
    return false;
}

bool Game::move(int x, int y) {
    auto copy = m_current.value();
    copy.x += x;
    copy.y += y;
    if (check_collision(copy)) {
        m_current.value() = copy;
        return true;
    }
    return false;
}

void Game::update() {
    switch (m_state) {
    case State::Play:
        update_play();
        break;
    default:
        break;
    }
}

void Game::update_play() {
    if (!m_current)
        return;
    if (m_frame % 24 == 0)
        down();
    m_frame++;
    m_frame %= 24;
}

void Game::update(SDL_Event &e) {
    switch (m_state) {
    case State::Start:
        update_start(e);
        break;
    case State::Play:
        update_play(e);
        break;
    case State::Pause:
        update_pause(e);
        break;
    case State::GameOver:
        update_gameover(e);
        break;
    default:
        break;
    }
}

void Game::update_start(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN)
        m_state = State::Play;
}

void Game::update_pause(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN)
        m_state = State::Play;
}

void Game::update_gameover(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN)
        reset();
}

void Game::update_play(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_UP:
            // 上キーで回転
            if (m_current && e.key.repeat == 0) {
                rotate();
            }
            break;
        case SDLK_SPACE:
            if (m_current && e.key.repeat == 0) {
                drop();
            }
            break;
        case SDLK_LEFT:
            if (m_current) {
                move(-1, 0);
            }
            break;
        case SDLK_RIGHT:
            if (m_current) {
                move(1, 0);
            }
            break;
        case SDLK_DOWN:
            if (m_current) {
                move(0, 1);
            }
            break;
        case SDLK_p:
        case SDLK_ESCAPE:
            m_state = State::Pause;
            break;
        }
    }
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

void Game::draw() const {
    switch (m_state) {
    case State::Start:
        draw_start();
        break;
    case State::Play:
        draw_play();
        break;
    case State::Pause:
        draw_pause();
        break;
    case State::GameOver:
        draw_gameover();
        break;
    default:
        draw_play();
        break;
    }
}

void Game::draw_board() const {
    for (int y = 0; y < ROW; y++) {
        for (int x = 0; x < COL; x++) {
            auto field = m_board.at(y).at(x);
            if (field) {
                SDL_Rect rect = {x * GS, y * GS, GS, GS};
                auto color = get_color(field.value());
                SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(m_renderer, &rect);
            }
        }
    }
}

void Game::draw_current() const {
    // currentの描画とdropした時のcurrentの位置を灰色で描画
    if (!m_current)
        return;
    auto tetrimino = m_current.value();
    auto blocks = tetrimino.get_blocks();
    int limity = limit_y();
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!blocks[y][x]) {
                continue;
            }
            {
                // drop側
                int nx = tetrimino.x + x;
                int ny = limity + y;
                SDL_Rect drop_rect = {nx * GS, ny * GS, GS, GS};
                SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
                SDL_RenderFillRect(m_renderer, &drop_rect);
            }
            {
                // current側
                int nx = tetrimino.x + x;
                int ny = tetrimino.y + y;
                SDL_Rect rect = {nx * GS, ny * GS, GS, GS};
                auto color = get_color(tetrimino.type);
                SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(m_renderer, &rect);
            }
        }
    }
}

void Game::draw_start() const {
    SDL_Rect rect = {(WIDTH - m_start_texture.w) / 2, (HEIGHT - m_start_texture.h) / 2,
                     m_start_texture.w, m_start_texture.h};
    SDL_RenderCopy(m_renderer, m_start_texture.texture, nullptr, &rect);
}

void Game::draw_pause() const {
    draw_board();
    draw_current();
    SDL_Rect rect = {(WIDTH - m_pause_texture.w) / 2, (HEIGHT - m_pause_texture.h) / 2,
                     m_pause_texture.w, m_pause_texture.h};
    SDL_RenderCopy(m_renderer, m_pause_texture.texture, nullptr, &rect);
}

void Game::draw_play() const {
    draw_board();
    draw_current();
}

void Game::draw_gameover() const {
    draw_board();
    SDL_Rect rect = {(WIDTH - m_gameover_texture.w) / 2, (HEIGHT - m_gameover_texture.h) / 2,
                     m_gameover_texture.w, m_gameover_texture.h};
    SDL_RenderCopy(m_renderer, m_gameover_texture.texture, nullptr, &rect);
}

bool Game::check_collision(const Tetrimino &t) const {
    auto blocks = t.get_blocks();
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!blocks.at(y).at(x))
                continue;
            int nx = x + t.x;
            int ny = y + t.y;
            // TODO: 初期位置のyは-1にすべき?
            if (!(0 <= nx && nx < COL && 0 <= ny && ny < ROW))
                return false;
            if (m_board.at(ny).at(nx))
                return false;
        }
    }
    return true;
}

std::vector<int> Game::check_lines() const {
    std::vector<int> result;
    for (int y = 0; y < ROW; y++) {
        bool f = true;
        for (int x = 0; x < COL; x++) {
            if (!m_board.at(y).at(x)) {
                f = false;
                break;
            }
        }
        if (f)
            result.push_back(y);
    }
    return result;
}

void Game::delete_lines() {
    auto lines = check_lines();
    for (auto y : lines) {
        for (int y1 = y; y1 >= 0; y1--) {
            for (int x = 0; x < COL; x++) {
                if (y1 == 0) {
                    // 一番上の行が揃った場合
                    m_board.at(y1).at(x) = std::nullopt;
                } else {
                    m_board.at(y1).at(x) = m_board.at(y1 - 1).at(x);
                }
            }
        }
    }
}

void Game::reset() {
    m_state = State::Play;
    m_board = std::array<std::array<std::optional<TetriminoType>, COL>, ROW>();
    new_tetrimino();
}
