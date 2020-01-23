#pragma once

#include "common.h"
#include "tetrimino.h"
#include <array>
#include <optional>

enum class State {
    Start,
    Play,
    Pause,
    GameOver,
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    TTF_Font *m_font;
    std::array<std::array<std::optional<TetriminoType>, COL>, ROW> m_board;
    std::optional<Tetrimino> m_current = std::nullopt;
    State m_state;

    int m_frame;

    void update();
    void update_play();

    void update(SDL_Event &e);
    void update_start(SDL_Event &e);
    void update_play(SDL_Event &e);
    void update_pause(SDL_Event &e);
    void update_gameover(SDL_Event &e);

    void draw() const;
    void draw_board() const;
    void draw_current() const;
    void draw_start() const;
    void draw_play() const;
    void draw_pause() const;
    void draw_gameover() const;

    /// 新しいテトリミノを用意。
    void new_tetrimino();
    /// テトリミノをm_boardに置く。置けることは保証されている。
    void set_tetrimino();
    /// テトリミノを置く。
    void put();
    /// テトリミノを回転させる。
    bool rotate();
    /// テトリミノを移動させる。
    bool move(int x, int y);
    /// 下方向への移動。テトリミノを置いたり新しいテトリミノをセットしたりもする。
    void down();
    /// m_currentを一番下まで落とした時のy座標。
    int limit_y() const;
    /// 一気に下まで落とす。
    void drop();
    /// 衝突チェック。trueなら衝突なし。
    bool check_collision(const Tetrimino &t) const;
    /// ゲームオーバーかチェック
    bool check_gameover() const;
    /// 揃った行を小さい順に返す。
    std::vector<int> check_lines() const;
    /// 揃った行を削除。
    void delete_lines();
    /// リセット
    void reset();

    Texture make_texture(const std::string &text, SDL_Color color);

    Texture m_start_texture;
    Texture m_pause_texture;
    Texture m_gameover_texture;

    // 新しいテトリミノを出す位置
    const int start_x = 4;
    const int start_y = 0;
};
