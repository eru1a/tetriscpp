#include "../src/tetrimino.cpp"
#include "gtest/gtest.h"
#include <algorithm>

// vectorの比較ってどうやるの?
bool blocks_equal(Blocks b1, Blocks b2) {
    if (b1.size() != b2.size())
        return false;
    std::sort(b1.begin(), b1.end());
    std::sort(b2.begin(), b2.end());
    for (size_t i = 0; i < b1.size(); i++) {
        if (b1[i] != b2[i])
            return false;
    }
    return true;
}

TEST(tetrimino, test) {
    // I
    Tetrimino i(0, 0, TetriminoType::I);
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {1, 0}, {2, 0}}, i.blocks()));
    i.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {0, 0}, {0, 1}, {0, 2}}, i.blocks()));
    i.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {1, 0}, {2, 0}}, i.blocks()));

    i.move(5, 5);
    ASSERT_TRUE(blocks_equal({{4, 5}, {5, 5}, {6, 5}, {7, 5}}, i.blocks()));
    i.rotate();
    ASSERT_TRUE(blocks_equal({{5, 4}, {5, 5}, {5, 6}, {5, 7}}, i.blocks()));

    // O
    Tetrimino o(0, 0, TetriminoType::O);
    ASSERT_TRUE(blocks_equal({{0, 0}, {0, 1}, {1, 0}, {1, 1}}, o.blocks()));
    o.rotate();
    ASSERT_TRUE(blocks_equal({{0, 0}, {0, 1}, {1, 0}, {1, 1}}, o.blocks()));
    o.rotate();
    ASSERT_TRUE(blocks_equal({{0, 0}, {0, 1}, {1, 0}, {1, 1}}, o.blocks()));
    o.rotate();
    ASSERT_TRUE(blocks_equal({{0, 0}, {0, 1}, {1, 0}, {1, 1}}, o.blocks()));

    o.move(5, 5);
    ASSERT_TRUE(blocks_equal({{5, 5}, {5, 6}, {6, 5}, {6, 6}}, o.blocks()));
    o.rotate();
    ASSERT_TRUE(blocks_equal({{5, 5}, {5, 6}, {6, 5}, {6, 6}}, o.blocks()));

    // S
    Tetrimino s(0, 0, TetriminoType::S);
    ASSERT_TRUE(blocks_equal({{1, 0}, {0, 0}, {0, 1}, {-1, 1}}, s.blocks()));
    s.rotate();
    ASSERT_TRUE(blocks_equal({{-1, -1}, {-1, 0}, {0, 0}, {0, 1}}, s.blocks()));
    s.rotate();
    ASSERT_TRUE(blocks_equal({{1, 0}, {0, 0}, {0, 1}, {-1, 1}}, s.blocks()));

    s.move(5, 5);
    ASSERT_TRUE(blocks_equal({{6, 5}, {5, 5}, {5, 6}, {4, 6}}, s.blocks()));
    s.rotate();
    ASSERT_TRUE(blocks_equal({{4, 4}, {4, 5}, {5, 5}, {5, 6}}, s.blocks()));

    // Z
    Tetrimino z(0, 0, TetriminoType::Z);
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {0, 1}, {1, 1}}, z.blocks()));
    z.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {-1, 1}, {0, 0}, {0, -1}}, z.blocks()));
    z.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {0, 1}, {1, 1}}, z.blocks()));

    z.move(5, 5);
    ASSERT_TRUE(blocks_equal({{4, 5}, {5, 5}, {5, 6}, {6, 6}}, z.blocks()));
    z.rotate();
    ASSERT_TRUE(blocks_equal({{4, 5}, {4, 6}, {5, 5}, {5, 4}}, z.blocks()));

    // J
    Tetrimino j(0, 0, TetriminoType::J);
    ASSERT_TRUE(blocks_equal({{-1, 0}, {-1, -1}, {0, 0}, {1, 0}}, j.blocks()));
    j.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {1, -1}, {0, 0}, {0, 1}}, j.blocks()));
    j.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {1, 0}, {1, 1}}, j.blocks()));
    j.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {0, 0}, {0, 1}, {-1, 1}}, j.blocks()));
    j.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {-1, -1}, {0, 0}, {1, 0}}, j.blocks()));

    j.move(5, 5);
    ASSERT_TRUE(blocks_equal({{4, 5}, {4, 4}, {5, 5}, {6, 5}}, j.blocks()));
    j.rotate();
    ASSERT_TRUE(blocks_equal({{5, 4}, {6, 4}, {5, 5}, {5, 6}}, j.blocks()));

    // L
    Tetrimino l(0, 0, TetriminoType::L);
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {1, 0}, {1, -1}}, l.blocks()));
    l.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {0, 0}, {0, 1}, {1, 1}}, l.blocks()));
    l.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {-1, 1}, {0, 0}, {1, 0}}, l.blocks()));
    l.rotate();
    ASSERT_TRUE(blocks_equal({{-1, -1}, {0, -1}, {0, 0}, {0, 1}}, l.blocks()));
    l.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {1, 0}, {1, -1}}, l.blocks()));

    l.move(5, 5);
    ASSERT_TRUE(blocks_equal({{4, 5}, {5, 5}, {6, 5}, {6, 4}}, l.blocks()));
    l.rotate();
    ASSERT_TRUE(blocks_equal({{5, 4}, {5, 5}, {5, 6}, {6, 6}}, l.blocks()));

    // T
    Tetrimino t(0, 0, TetriminoType::T);
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {0, -1}, {1, 0}}, t.blocks()));
    t.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {0, 0}, {1, 0}, {0, 1}}, t.blocks()));
    t.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {0, 1}, {1, 0}}, t.blocks()));
    t.rotate();
    ASSERT_TRUE(blocks_equal({{0, -1}, {0, 0}, {-1, 0}, {0, 1}}, t.blocks()));
    t.rotate();
    ASSERT_TRUE(blocks_equal({{-1, 0}, {0, 0}, {0, -1}, {1, 0}}, t.blocks()));

    t.move(5, 5);
    ASSERT_TRUE(blocks_equal({{4, 5}, {5, 5}, {5, 4}, {6, 5}}, t.blocks()));
    t.rotate();
    ASSERT_TRUE(blocks_equal({{5, 4}, {5, 5}, {6, 5}, {5, 6}}, t.blocks()));
}
