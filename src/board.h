#pragma once

#include "tetrimino.h"
#include <array>
#include <optional>

const int col = 10;
const int row = 20;

using Board_t = std::array<std::array<std::optional<TetriminoType>, col>, row>;

class Board {
public:
    Board() {}
    Board(const Board_t &board) : board(board) {}
    ~Board() {}

    Board_t board;

    /// リセット。
    void reset();
    /// テトリミノを置く。置けることが保証されていなければいけない。
    void put(const Tetrimino &t);
    /// 一気に下まで落とす。置けることが保証されていなければいけない。
    void drop(const Tetrimino &t);
    /// 衝突と境界のチェック。trueなら問題あり。
    bool detect_collision(const Tetrimino &t) const;
    /// テトリミノを一番下まで落とした時のy座標。既にこれ以上落とせない場合は-1。
    int limit_y(const Tetrimino &t) const;
    /// ゲームオーバーかチェック。
    /// 渡されるテトリミノの座標は(start_x, start_y)である必要がある。
    bool check_gameover(const Tetrimino &t) const;
    /// 揃った行を小さい順に返す。
    std::vector<int> complete_lines() const;
    /// 揃った行を削除。
    void delete_lines();
};
