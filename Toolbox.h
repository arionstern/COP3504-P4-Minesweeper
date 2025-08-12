//
// Created by arist on 11/29/2024.
//

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Button.h" // Include Button class header for button management
#include "Tile.h"

class Toolbox {
    friend class Tile;
    friend class GameState;
    friend class Button;
public:

    sf::RenderWindow window;


    GameState* gameState;

    // Buttons
    Button* debugButton;   // Reveals mines in debug mode
    Button* newGameButton; // Resets/starts a new game
    Button* testButton1;   // Loads test board #1
    Button* testButton2;   // Loads test board #2
    Button* testButton3;

    // Textures
    sf::Texture tileHiddenTexture;
    sf::Texture tileRevealedTexture;
    sf::Texture numberTextures[8];
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    sf::Texture faceWinTexture;
    sf::Texture faceLoseTexture;

    sf::Texture faceNeutralTexture;




    sf::Texture digitTexture;
    std::vector<sf::Sprite> mineCounterSprites;

    int mineCounter; // Tracks num of mines remaining

    void initializeMineCounter(int totalMines) { mineCounter = totalMines; }
    void decreaseMineCounter() { --mineCounter; }
    void increaseMineCounter() { ++mineCounter; }



    bool debugMode; // Tracks whether debug mode is active

    bool tileHasMine(int x, int y) const;
    void revealTileForDebug(int x, int y);

    void drawDigits(int number, sf::Vector2f position);

    // Singleton instance accessor
    static Toolbox& getInstance();

    void checkWinCondition();

private:
    // Private constructor
    Toolbox();
    ~Toolbox();

    // Deleted copy constructor and assignment operator
    Toolbox(const Toolbox&) = delete;
    Toolbox& operator=(const Toolbox&) = delete;

    void cleanup();
};
#endif //TOOLBOX_H
