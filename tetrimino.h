#pragma once

#include <vector>

enum class TetriminoType : int {
    I,
    O,
    S,
    Z,
    J,
    L,
    T,
};

using Blocks = std::vector<std::vector<bool>>;
// arrayにしたいけど初期化方法がよく分からない
// using Blocks = std::array<std::array<bool, 4>, 4>;

struct Tetrimino {
    int x, y, r;
    TetriminoType type;
    void rotate();
    Blocks get_blocks() const;
};
