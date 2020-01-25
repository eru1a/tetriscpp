#pragma once

#include "board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int gs = 30;
const int width = gs * col;
const int height = gs * row;

struct Texture {
    SDL_Texture *texture;
    int w, h;
};

enum class GameState {
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
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    Board board;
    Tetrimino current;
    int frame;
    GameState state;

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
    void draw_current_shadow() const;
    void draw_start() const;
    void draw_play() const;
    void draw_pause() const;
    void draw_gameover() const;

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
