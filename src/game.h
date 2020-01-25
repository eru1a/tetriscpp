#pragma once

#include "board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int gs = 30;
const int width = gs * col;
const int height = gs * row;

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Board board;
    Tetrimino current;
    int frame;

    void update();
    void update(SDL_Event &e);

    void draw() const;
    void draw_board() const;
    void draw_current() const;
    void draw_current_shadow() const;

    /// 新しいテトリミノを用意。揃った行の削除とゲームオーバーかの判定も行う。
    void new_tetrimino();
    /// テトリミノを置く。置けることは保証されている。
    void set_tetrimino();
    /// 下方向への強制移動。移動出来なければそこへ置き新しいテトリミノをセットする。
    void down();
    /// 一気に下まで落とす。
    void drop();
    /// テトリミノを移動させる。
    bool move(int x, int y);
    /// テトリミノを回転させる。
    bool rotate();
    /// リセット
    void reset();
};
