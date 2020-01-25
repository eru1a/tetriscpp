#include "board.h"
#include <algorithm>
#include <cassert>

void Board::reset() { board = Board_t{}; }

void Board::put(const Tetrimino &t) {
    for (const auto [x, y] : t.blocks()) {
        assert(!board.at(y).at(x).has_value());
        board.at(y).at(x) = t.type();
    }
}

void Board::drop(const Tetrimino &t) {
    auto [x, y] = t.pos();
    int ly = limit_y(t);
    assert(ly >= y);
    auto copy = t;
    copy.set_pos(x, ly);
    put(copy);
}

bool Board::detect_collision(const Tetrimino &t) const {
    for (const auto [x, y] : t.blocks()) {
        if (x < 0 || x >= col)
            return true;
        if (y < 0 || y >= row)
            return true;
        if (board.at(y).at(x))
            return true;
    }
    return false;
}

int Board::limit_y(const Tetrimino &t) const {
    auto t1 = t;
    if (detect_collision(t1))
        return -1;
    while (!detect_collision(t1)) {
        t1.move(0, 1);
    }
    auto [x, y] = t1.pos();
    return y - 1;
}

bool Board::check_gameover(const Tetrimino &t) const {
    // テトリミノがある位置に他のブロックがあるならゲームオーバー。
    // テトリミノを一番下まで落とした時に上にはみ出るならゲームオーバー。
    // (今は初期値のy座標が1だから上にはみ出ることはない)
    if (detect_collision(t))
        return true;
    return false;
}

std::vector<int> Board::complete_lines() const {
    auto is_complete = [](auto &line) -> bool {
        return std::all_of(line.begin(), line.end(),
                           [](const auto &field) { return field.has_value(); });
    };
    std::vector<int> lines;
    for (int y = 0; y < row; y++) {
        if (is_complete(board.at(y))) {
            lines.push_back(y);
        }
    }
    return lines;
}

void Board::delete_lines() {
    auto lines = complete_lines();
    for (auto l : lines) {
        for (int y = l; y >= 0; y--) {
            for (int x = 0; x < col; x++) {
                if (y == 0) {
                    // 一番上の行が揃った場合
                    board.at(y).at(x) = std::nullopt;
                } else {
                    board.at(y).at(x) = board.at(y - 1).at(x);
                }
            }
        }
    }
}
