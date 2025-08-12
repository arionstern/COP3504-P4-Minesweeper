//
// Created by arist on 11/29/2024.
//

#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <SFML/Graphics.hpp>
#include "Toolbox.h"
#include "GameState.h"
#include "Tile.h"


// textures and other resources
extern sf::Texture tileHiddenTexture;
extern sf::Texture tileRevealedTexture;
extern sf::Texture numberTextures[8];
extern sf::Texture flagTexture;
extern sf::Texture mineTexture;

// other global game state variables
extern bool debugMode;

int launch();
void restart();
void render();
void toggleDebugMode();
bool getDebugMode();
int gameLoop();
#endif //MINESWEEPER_H
