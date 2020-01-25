#include "tetrimino.h"
#include <algorithm>

struct Block {
    /// 回転数
    int rotate;
    // (0, 0)にブロックがあるとした時のブロックの相対座標
    Blocks blocks;
};

Block block[8] = {
    // I
    // □□□□
    {2, {{-1, 0}, {0, 0}, {1, 0}, {2, 0}}},
    // O
    // □□
    // □□
    {1, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
    // S
    //   □□
    // □□
    {2, {{1, 0}, {0, 0}, {0, 1}, {-1, 1}}},
    // Z
    // □□
    //   □□
    {2, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}},
    // J
    // □
    // □□□
    {4, {{-1, 0}, {-1, -1}, {0, 0}, {1, 0}}},
    // L
    //     □
    // □□□
    {4, {{-1, 0}, {0, 0}, {1, 0}, {1, -1}}},
    // T
    //   □
    // □□□
    {4, {{-1, 0}, {0, 0}, {0, -1}, {1, 0}}},
};

Pos Tetrimino::pos() const { return {m_x, m_y}; }
void Tetrimino::set_pos(int x, int y) {
    m_x = x;
    m_y = y;
}
TetriminoType Tetrimino::type() const { return m_type; }

Blocks Tetrimino::blocks() const {
    const Block &b = block[static_cast<int>(m_type)];
    Blocks ret;
    std::for_each(b.blocks.begin(), b.blocks.end(), [&](const auto &p) {
        auto [dx, dy] = p;
        // (cos270 -sin270) (dx)
        // (sin270  cos270) (dy)
        for (int i = 0; i < m_r; i++) {
            int tmp = dx;
            dx = -dy;
            dy = tmp;
        }
        ret.emplace_back(m_x + dx, m_y + dy);
    });
    return ret;
}

void Tetrimino::move(int x, int y) {
    m_x += x;
    m_y += y;
}

void Tetrimino::rotate() {
    m_r++;
    m_r %= block[static_cast<int>(m_type)].rotate;
}
