#include "../src/board.h"
#include "../src/tetrimino.h"
#include "gtest/gtest.h"
#include <sstream>

Board board_from_string(const std::string &str) {
    Board_t board;
    int x = 0;
    int y = 0;
    for (auto c : str) {
        switch (c) {
        case '\n':
            y++;
            x = 0;
            break;
        case '.':
            x++;
            break;
        case 'I':
            board.at(y).at(x) = TetriminoType::I;
            x++;
            break;
        case 'O':
            board.at(y).at(x) = TetriminoType::O;
            x++;
            break;
        case 'S':
            board.at(y).at(x) = TetriminoType::S;
            x++;
            break;
        case 'Z':
            board.at(y).at(x) = TetriminoType::Z;
            x++;
            break;
        case 'J':
            board.at(y).at(x) = TetriminoType::J;
            x++;
            break;
        case 'L':
            board.at(y).at(x) = TetriminoType::L;
            x++;
            break;
        case 'T':
            board.at(y).at(x) = TetriminoType::T;
            x++;
            break;
        default:
            assert(false);
        }
    }
    return Board(board);
}

std::string board_to_string(const Board &b) {
    std::stringstream str;
    Board_t board = b.board;
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            auto field = board.at(y).at(x);
            if (field) {
                switch (field.value()) {
                case TetriminoType::I:
                    str << "I";
                    break;
                case TetriminoType::O:
                    str << "O";
                    break;
                case TetriminoType::S:
                    str << "S";
                    break;
                case TetriminoType::Z:
                    str << "Z";
                    break;
                case TetriminoType::J:
                    str << "J";
                    break;
                case TetriminoType::L:
                    str << "L";
                    break;
                case TetriminoType::T:
                    str << "T";
                    break;
                }
            } else {
                str << ".";
            }
        }
        str << "\n";
    }
    return str.str();
}

TEST(Board, put) {
    Board board;

    Tetrimino i(4, 1, TetriminoType::I);
    board.put(i);
    i.rotate();
    i.move(4, 10);
    board.put(i);

    Tetrimino s(5, 5, TetriminoType::S);
    board.put(s);

    Tetrimino t(1, 18, TetriminoType::T);
    t.rotate();
    t.rotate();
    t.rotate();
    board.put(t);

    std::string str = "..........\n"
                      "...IIII...\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      ".....SS...\n"
                      "....SS....\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      ".T........\n"
                      "TT........\n"
                      ".T........\n";
    EXPECT_EQ(str, board_to_string(board));
}

TEST(board, drop) {
    Board board;

    Tetrimino i(TetriminoType::I);
    board.drop(i);

    Tetrimino o(TetriminoType::O);
    o.rotate();
    o.move(-1, 0);
    board.drop(o);

    Tetrimino t(TetriminoType::T);
    t.rotate();
    t.rotate();
    t.move(-3, 0);
    board.drop(t);

    Tetrimino l(TetriminoType::L);
    l.rotate();
    l.rotate();
    l.move(3, 0);
    board.drop(l);

    Tetrimino s(TetriminoType::S);
    s.rotate();
    s.move(5, 0);
    board.drop(s);

    std::string str = "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "........S.\n"
                      "........SS\n"
                      "...OO.LLLS\n"
                      "TTTOO.L...\n"
                      ".T.IIII...\n";
    EXPECT_EQ(str, board_to_string(board));
}

TEST(boad, detect_collision) {
    std::string str = "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "....S.....\n"
                      "....SS....\n"
                      ".....S....\n"
                      "....OO....\n"
                      "....OO....\n";
    Board b = board_from_string(str);

    Tetrimino i(TetriminoType::I);
    // 左右の壁
    i.set_pos(0, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.set_pos(1, 0);
    EXPECT_EQ(false, b.detect_collision(i));
    i.set_pos(7, 0);
    EXPECT_EQ(false, b.detect_collision(i));
    i.set_pos(8, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    // 上下の壁
    i.rotate();
    i.set_pos(1, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(0, 1);
    EXPECT_EQ(false, b.detect_collision(i));
    i.set_pos(1, 17);
    EXPECT_EQ(false, b.detect_collision(i));
    i.move(0, 1);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(0, 1);
    EXPECT_EQ(true, b.detect_collision(i));
    // ブロック同士
    i.rotate();
    i.set_pos(1, 17);
    EXPECT_EQ(false, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(false, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(true, b.detect_collision(i));
    i.move(1, 0);
    EXPECT_EQ(false, b.detect_collision(i));

    Tetrimino t(3, 17, TetriminoType::T);
    EXPECT_EQ(false, b.detect_collision(t));
    t.rotate();
    EXPECT_EQ(false, b.detect_collision(t));
    t.rotate();
    EXPECT_EQ(false, b.detect_collision(t));
    t.move(0, -1);
    EXPECT_EQ(true, b.detect_collision(t));
}

TEST(board, limit_y) {
    std::string str = "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "........I.\n"
                      "....S...I.\n"
                      "....SS..I.\n"
                      ".....S..I.\n"
                      "....OO..I.\n"
                      "....OO..I.\n";
    Board b = board_from_string(str);

    Tetrimino i(TetriminoType::I);
    i.set_pos(1, 1);
    EXPECT_EQ(19, b.limit_y(i));
    i.move(1, 0);
    EXPECT_EQ(14, b.limit_y(i));

    Tetrimino o(TetriminoType::O);
    o.set_pos(5, 1);
    EXPECT_EQ(14, b.limit_y(o));
    o.move(1, 0);
    EXPECT_EQ(18, b.limit_y(o));
    o.move(1, 0);
    EXPECT_EQ(2, b.limit_y(o));

    std::string str2 = "..........\n"
                       "..........\n"
                       "....JJ....\n"
                       "....J.....\n"
                       "....J.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....I.....\n"
                       "....S.....\n"
                       "....SS....\n"
                       ".....S....\n"
                       "....OO....\n"
                       "....OO....\n";
    Board b2 = board_from_string(str2);

    Tetrimino z2(4, 1, TetriminoType::Z);
    EXPECT_EQ(-1, b2.limit_y(z2));
    z2.rotate();
    EXPECT_EQ(1, b2.limit_y(z2));
}

TEST(board, check_gameover) {
    std::string str = "..........\n"
                      "..........\n"
                      "...S......\n"
                      "...SS.....\n"
                      "....S.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....I.....\n"
                      "....S.....\n"
                      "....SS....\n"
                      ".....S....\n"
                      "....OO....\n"
                      "....OO....\n";
    Board b = board_from_string(str);

    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::I)));
    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::O)));
    EXPECT_EQ(true, b.check_gameover(Tetrimino(TetriminoType::S)));
    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::Z)));
    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::J)));
    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::L)));
    EXPECT_EQ(false, b.check_gameover(Tetrimino(TetriminoType::T)));
}

TEST(board, complete_lines) {
    std::string str = "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "....T.....\n"
                      "....TT....\n"
                      "....TLL...\n"
                      "TTTOOILLLL\n"
                      ".T.OOILL..\n"
                      "IIJJJILLLI\n"
                      "IIJJJIL.ZI\n"
                      "IIJOOOOZZI\n"
                      "IIJOOOOZ.I\n";
    Board b = board_from_string(str);

    std::vector<int> lines = {14, 16, 18};
    EXPECT_EQ(lines, b.complete_lines());
}

TEST(board, delete_lines) {
    std::string str = "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "..........\n"
                      "....T.....\n"
                      "....TT....\n"
                      "....TLL...\n"
                      "TTTOOILLLL\n"
                      ".T.OOILL..\n"
                      "IIJJJILLLI\n"
                      "IIJJJIL.ZI\n"
                      "IIJOOOOZZI\n"
                      "IIJOOOOZ.I\n";
    Board b = board_from_string(str);

    b.delete_lines();

    std::string str2 = "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "..........\n"
                       "....T.....\n"
                       "....TT....\n"
                       "....TLL...\n"
                       ".T.OOILL..\n"
                       "IIJJJIL.ZI\n"
                       "IIJOOOOZ.I\n";
    EXPECT_EQ(str2, board_to_string(b));
}
