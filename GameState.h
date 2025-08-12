//
// Created by arist on 11/29/2024.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SFML/System.hpp>  // For sf::Vector2i
#include <vector>
#include "Tile.h"           // To use Tile objects

class GameState
{
    friend class Tile;
    friend class Toolbox;
    friend void toggleDebugMode();
public:
    // Enumeration for play status
    enum PlayStatus { WIN, LOSS, PLAYING };

private:
    // Attributes
    sf::Vector2i _dimensions;          // Dimensions of the board
    int _numberOfMines;                // Num mines on board
    int _flagCount;
    PlayStatus _status;                // Current play status
    std::vector<std::vector<Tile>> _board; // 2D grid of Tile objects


public:
    GameState(sf::Vector2i _dimensions = sf::Vector2i(25, 16), int _numberOfMines = 50);
    GameState(const char* filepath);

    int getFlagCount();
    int getMineCount();
    Tile* getTile(int x, int y);
    PlayStatus getPlayStatus();
    void setPlayStatus(PlayStatus _status);




};
#endif //GAMESTATE_H
