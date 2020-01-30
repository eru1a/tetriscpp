#pragma once

#include "board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <deque>

const int gs = 30;
const int board_width = gs * col;
const int board_height = gs * row;
const int next_area_width = gs * 4;
const int next_area_height = gs * 2;
const int margin = 10;
const int font_height = 36;
const int width = board_width + margin + next_area_width + margin;
const int height = board_height;

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
    // queueだとiterate出来ないのでdeque
    std::deque<TetriminoType> next;
    int frame = 0;
    GameState state = GameState::Start;
    int score = 0;
    bool delete_animation = false;
    int delete_animation_frame = 0;

    // 描画する文字等のtopleft
    // viewport使ったほうがいいのかな
    std::pair<int, int> next_text_topleft = {board_width + margin, 0};
    std::vector<std::pair<int, int>> next_topleft = {
        {board_width + margin, font_height + next_area_height * 0 + margin * 1},
        {board_width + margin, font_height + next_area_height * 1 + margin * 2},
        {board_width + margin, font_height + next_area_height * 2 + margin * 3},
    };
    std::pair<int, int> score_text_topleft = {board_width + margin,
                                              font_height + next_area_height * 3 + margin * 4};
    std::pair<int, int> score_topleft = {board_width + margin,
                                         font_height * 2 + next_area_height * 3 + margin * 5};

    void update();
    void update_play();

    void update(SDL_Event &e);
    void update_start(SDL_Event &e);
    void update_play(SDL_Event &e);
    void update_delete_animation(SDL_Event &e);
    void update_pause(SDL_Event &e);
    void update_gameover(SDL_Event &e);

    void draw() const;
    void draw_board() const;
    void draw_tetrimino(const Tetrimino &t, std::optional<std::pair<int, int>> topleft) const;
    void draw_current() const;
    void draw_current_shadow() const;
    void draw_next() const;
    void draw_score() const;
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
    /// テトリミノを置いた時に行う処理
    void on_tetrimino_set();
    /// テトリミノを移動させる。
    bool move(int x, int y);
    /// テトリミノを回転させる。
    bool rotate();
    /// リセット
    void reset();
};
