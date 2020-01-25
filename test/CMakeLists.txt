project(test)

add_subdirectory(googletest/googletest)

add_library(tetrislib ../src/tetrimino.cpp ../src/board.cpp)

add_executable(${PROJECT_NAME} tetrimino_test.cpp board_test.cpp)

target_link_libraries(${PROJECT_NAME} tetrislib gtest_main)
