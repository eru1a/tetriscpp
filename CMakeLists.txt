project(tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --pedantic-error")

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)

add_executable(${PROJECT_NAME}
  main.cpp
  common.cpp
  game.cpp
  tetrimino.cpp
)

file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})
