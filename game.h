#pragma once

#include "common.h"
#include "tetrimino.h"
#include <array>
#include <optional>

enum class State {
    Play,
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
    std::array<std::array<std::optional<TetriminoType>, COL>, ROW> m_board;
    std::optional<Tetrimino> m_current = std::nullopt;
    State m_state = State::Play;

    int m_frame;

    void update();
    void update(SDL_Event &e);
    void draw() const;

    /// 新しいテトリミノを用意。
    void new_tetrimino();
    /// テトリミノをm_boardに置く。置けることは保証されている。
    void set_tetrimino();
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
    /// 揃った行を大きい順に返す。
    std::vector<int> check_lines() const;
    /// 揃った行を削除。
    void delete_lines();
};
