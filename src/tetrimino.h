#pragma once

#include <vector>

using Pos = std::pair<int, int>;
using Blocks = std::vector<std::pair<int, int>>;

enum class TetriminoType : int {
    I,
    O,
    S,
    Z,
    J,
    L,
    T,
};

class Tetrimino {
public:
    // 開始位置
    static const int start_x = 4;
    // TODO: 0にすべき?
    static const int start_y = 1;

    Tetrimino() : m_x(start_x), m_y(start_y), m_r(0), m_type(TetriminoType::I) {}
    Tetrimino(TetriminoType type) : m_x(start_x), m_y(start_y), m_r(0), m_type(type) {}
    Tetrimino(int x, int y, TetriminoType type) : m_x(x), m_y(y), m_r(0), m_type(type) {}
    /// (x, y)を返す
    Pos pos() const;
    /// 座標を(x, y)にセット
    void set_pos(int x, int y);
    /// ブロックのタイプを返す
    TetriminoType type() const;
    /// ブロックの座標を返す
    Blocks blocks() const;
    /// 移動させる
    void move(int x, int y);
    /// 時計回りに90度回転させる
    void rotate();

private:
    int m_x;
    int m_y;
    int m_r;
    TetriminoType m_type;
};
