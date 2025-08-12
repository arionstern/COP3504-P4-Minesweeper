cmake_minimum_required(VERSION 3.29)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)





add_executable(minesweeper main.cpp
        minesweeper.h
        Button.h
        GameState.h
        Tile.h
        Toolbox.h
        Tile.cpp
        Toolbox.cpp
        Gamestate.cpp
        Button.cpp)

target_link_libraries(minesweeper sfml-graphics sfml-audio)

