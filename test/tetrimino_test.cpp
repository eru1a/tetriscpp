#include "../src/tetrimino.cpp"
#include "gtest/gtest.h"
#include <algorithm>

TEST(tetrimino, test) {
    // I
    Tetrimino i(0, 0, TetriminoType::I);
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {1, 0}, {2, 0}}), i.blocks());
    i.rotate();
    EXPECT_EQ(Blocks({{0, -1}, {0, 0}, {0, 1}, {0, 2}}), i.blocks());
    i.rotate();
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {1, 0}, {2, 0}}), i.blocks());

    i.move(5, 5);
    EXPECT_EQ(Blocks({{4, 5}, {5, 5}, {6, 5}, {7, 5}}), i.blocks());
    i.rotate();
    EXPECT_EQ(Blocks({{5, 4}, {5, 5}, {5, 6}, {5, 7}}), i.blocks());

    // O
    Tetrimino o(0, 0, TetriminoType::O);
    EXPECT_EQ(Blocks({{0, 0}, {0, 1}, {1, 0}, {1, 1}}), o.blocks());
    o.rotate();
    EXPECT_EQ(Blocks({{0, 0}, {0, 1}, {1, 0}, {1, 1}}), o.blocks());
    o.rotate();
    EXPECT_EQ(Blocks({{0, 0}, {0, 1}, {1, 0}, {1, 1}}), o.blocks());
    o.rotate();
    EXPECT_EQ(Blocks({{0, 0}, {0, 1}, {1, 0}, {1, 1}}), o.blocks());

    o.move(5, 5);
    EXPECT_EQ(Blocks({{5, 5}, {5, 6}, {6, 5}, {6, 6}}), o.blocks());
    o.rotate();
    EXPECT_EQ(Blocks({{5, 5}, {5, 6}, {6, 5}, {6, 6}}), o.blocks());

    // S
    Tetrimino s(0, 0, TetriminoType::S);
    EXPECT_EQ(Blocks({{1, 0}, {0, 0}, {0, 1}, {-1, 1}}), s.blocks());
    s.rotate();
    EXPECT_EQ(Blocks({{0, 1}, {0, 0}, {-1, 0}, {-1, -1}}), s.blocks());
    s.rotate();
    EXPECT_EQ(Blocks({{1, 0}, {0, 0}, {0, 1}, {-1, 1}}), s.blocks());

    s.move(5, 5);
    EXPECT_EQ(Blocks({{6, 5}, {5, 5}, {5, 6}, {4, 6}}), s.blocks());
    s.rotate();
    EXPECT_EQ(Blocks({{5, 6}, {5, 5}, {4, 5}, {4, 4}}), s.blocks());

    // Z
    Tetrimino z(0, 0, TetriminoType::Z);
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {0, 1}, {1, 1}}), z.blocks());
    z.rotate();
    EXPECT_EQ(Blocks({{0, -1}, {0, 0}, {-1, 0}, {-1, 1}}), z.blocks());
    z.rotate();
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {0, 1}, {1, 1}}), z.blocks());

    z.move(5, 5);
    EXPECT_EQ(Blocks({{4, 5}, {5, 5}, {5, 6}, {6, 6}}), z.blocks());
    z.rotate();
    EXPECT_EQ(Blocks({{5, 4}, {5, 5}, {4, 5}, {4, 6}}), z.blocks());

    // J
    Tetrimino j(0, 0, TetriminoType::J);
    EXPECT_EQ(Blocks({{-1, 0}, {-1, -1}, {0, 0}, {1, 0}}), j.blocks());
    j.rotate();
    EXPECT_EQ(Blocks({{0, -1}, {1, -1}, {0, 0}, {0, 1}}), j.blocks());
    j.rotate();
    EXPECT_EQ(Blocks({{1, 0}, {1, 1}, {0, 0}, {-1, 0}}), j.blocks());
    j.rotate();
    EXPECT_EQ(Blocks({{0, 1}, {-1, 1}, {0, 0}, {0, -1}}), j.blocks());
    j.rotate();
    EXPECT_EQ(Blocks({{-1, 0}, {-1, -1}, {0, 0}, {1, 0}}), j.blocks());

    j.move(5, 5);
    EXPECT_EQ(Blocks({{4, 5}, {4, 4}, {5, 5}, {6, 5}}), j.blocks());
    j.rotate();
    EXPECT_EQ(Blocks({{5, 4}, {6, 4}, {5, 5}, {5, 6}}), j.blocks());

    // L
    Tetrimino l(0, 0, TetriminoType::L);
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {1, 0}, {1, -1}}), l.blocks());
    l.rotate();
    EXPECT_EQ(Blocks({{0, -1}, {0, 0}, {0, 1}, {1, 1}}), l.blocks());
    l.rotate();
    EXPECT_EQ(Blocks({{1, 0}, {0, 0}, {-1, 0}, {-1, 1}}), l.blocks());
    l.rotate();
    EXPECT_EQ(Blocks({{0, 1}, {0, 0}, {0, -1}, {-1, -1}}), l.blocks());
    l.rotate();
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {1, 0}, {1, -1}}), l.blocks());

    l.move(5, 5);
    EXPECT_EQ(Blocks({{4, 5}, {5, 5}, {6, 5}, {6, 4}}), l.blocks());
    l.rotate();
    EXPECT_EQ(Blocks({{5, 4}, {5, 5}, {5, 6}, {6, 6}}), l.blocks());

    // T
    Tetrimino t(0, 0, TetriminoType::T);
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {0, -1}, {1, 0}}), t.blocks());
    t.rotate();
    EXPECT_EQ(Blocks({{0, -1}, {0, 0}, {1, 0}, {0, 1}}), t.blocks());
    t.rotate();
    EXPECT_EQ(Blocks({{1, 0}, {0, 0}, {0, 1}, {-1, 0}}), t.blocks());
    t.rotate();
    EXPECT_EQ(Blocks({{0, 1}, {0, 0}, {-1, 0}, {0, -1}}), t.blocks());
    t.rotate();
    EXPECT_EQ(Blocks({{-1, 0}, {0, 0}, {0, -1}, {1, 0}}), t.blocks());

    t.move(5, 5);
    EXPECT_EQ(Blocks({{4, 5}, {5, 5}, {5, 4}, {6, 5}}), t.blocks());
    t.rotate();
    EXPECT_EQ(Blocks({{5, 4}, {5, 5}, {6, 5}, {5, 6}}), t.blocks());
}
