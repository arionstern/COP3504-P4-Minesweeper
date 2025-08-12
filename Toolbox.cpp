//
// Created by arist on 11/30/2024.
//
#include "Toolbox.h"

#include <fstream>
#include <iostream>

#include "minesweeper.h"





void restartGame() {
    Toolbox& toolbox = Toolbox::getInstance();
    delete toolbox.gameState;
    toolbox.gameState = new GameState(sf::Vector2i(25, 16), 50);
    toolbox.debugMode = false;
    std::cout << "Game restarted!" << std::endl;
}

void loadTestBoard1() {
    std::ifstream file("boards/testboard1.brd");
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << "boards/testboard1.brd" << std::endl;
        return;
    }


    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    try {
        Toolbox& toolbox = Toolbox::getInstance();
        delete toolbox.gameState;
        toolbox.gameState = nullptr;
        toolbox.gameState = new GameState("boards/testboard1.brd");
        toolbox.debugMode = false; // Turn off debug mode
        std::cout << "Loaded Test Board 1 successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error while loading test board 1: " << e.what() << std::endl;
    }

}

void loadTestBoard2() {
    Toolbox& toolbox = Toolbox::getInstance();
    delete toolbox.gameState;
    toolbox.gameState = new GameState("boards/testboard2.brd");
    toolbox.debugMode = false; // Turn off debug mode
    std::cout << "Loaded Test Board 2!" << std::endl;

}

void loadTestBoard3() {
    Toolbox& toolbox = Toolbox::getInstance();
    delete toolbox.gameState;
    toolbox.gameState = new GameState("boards/testboard3.brd");
    toolbox.debugMode = false; // Turn off debug mode
    std::cout << "Loaded Test Board 2!" << std::endl;

}

Toolbox& Toolbox::getInstance() {
    static Toolbox instance; // Singleton instance
    return instance;
}



Toolbox::Toolbox()
    : gameState(nullptr), debugButton(nullptr), newGameButton(nullptr), testButton1(nullptr), testButton2(nullptr), debugMode(false) {
    // Initialize SFML Window
    window.create(sf::VideoMode(800, 600), "Minesweeper");

    for (int i = 0; i < 8; ++i) {
        if (!numberTextures[i].loadFromFile("images/number_" + std::to_string(i + 1) + ".png")) {
            std::cerr << "Failed to load number_" << (i + 1) << ".png!" << std::endl;
            std::exit(-1);
        }
    }

    // Initialize Buttons
    debugButton = new Button(sf::Vector2f(496, 514), toggleDebugMode);
    newGameButton = new Button(sf::Vector2f(368, 514), restartGame);
    testButton1 = new Button(sf::Vector2f(560, 514), loadTestBoard1);
    testButton2 = new Button(sf::Vector2f(624, 514), loadTestBoard2);

    // Set sprites for buttons
    sf::Texture* debugTexture = new sf::Texture();
    if (debugTexture->loadFromFile("images/debug.png")) {
        sf::Sprite* debugSprite = new sf::Sprite(*debugTexture);
        debugButton->setSprite(debugSprite);
    }

    sf::Texture* newGameTexture = new sf::Texture();
    if (newGameTexture->loadFromFile("images/face_happy.png")) {
        sf::Sprite* newGameSprite = new sf::Sprite(*newGameTexture);
        newGameButton->setSprite(newGameSprite);
    }

    sf::Texture* test1Texture = new sf::Texture();
    if (test1Texture->loadFromFile("images/test_1.png")) {
        sf::Sprite* test1Sprite = new sf::Sprite(*test1Texture);
        testButton1->setSprite(test1Sprite);
    }

    sf::Texture* test2Texture = new sf::Texture();
    if (test2Texture->loadFromFile("images/test_2.png")) {
        sf::Sprite* test2Sprite = new sf::Sprite(*test2Texture);
        testButton2->setSprite(test2Sprite);
    }
    sf::Texture* test3Texture = new sf::Texture();

    if (test3Texture->loadFromFile("images/Test_3.png")) {
        sf::Sprite* test3Sprite = new sf::Sprite(*test3Texture);
        testButton3 = new Button(sf::Vector2f(688, 514), loadTestBoard3); // Position near other buttons
        testButton3->setSprite(test3Sprite);
    } else {
        std::cerr << "Failed to load images/Test_3.png!" << std::endl;
        delete test3Texture;
        testButton3 = nullptr;
    }


    if (!tileHiddenTexture.loadFromFile("images/tile_hidden.png")) {
        std::cerr << "Failed to load tile_hidden.png!" << std::endl;
        std::exit(-1);
    }

    // Load revealed tile texture
    if (!tileRevealedTexture.loadFromFile("images/tile_revealed.png")) {
        std::cerr << "Failed to load tile_revealed.png!" << std::endl;
        std::exit(-1);
    }
    // Load flag texture
    if (!flagTexture.loadFromFile("images/flag.png")) {
        std::cerr << "Failed to load flag.png!" << std::endl;
        std::exit(-1); // Exit if the texture is not loaded
    }

    // Load mine texture
    if (!mineTexture.loadFromFile("images/mine.png")) {
        std::cerr << "Failed to load mine.png!" << std::endl;
        std::exit(-1); // Exit if the texture is not loaded
    }

    if (!digitTexture.loadFromFile("images/digits.png")) {
        std::cerr << "Failed to load digits.png!" << std::endl;
        std::exit(-1); // Exit if the texture is not loaded
    }

    if (!faceWinTexture.loadFromFile("images/face_win.png")) {
        throw std::runtime_error("Failed to load face_win.png");
    }
    if (!faceLoseTexture.loadFromFile("images/face_lose.png")) {
        throw std::runtime_error("Failed to load face_lose.png");
    }

    if (!faceNeutralTexture.loadFromFile("images/face_happy.png")) {
        throw std::runtime_error("Failed to load face_neutral.png");
    }

}




Toolbox::~Toolbox() {
    cleanup();
}

void Toolbox::cleanup() {
    delete gameState;
    delete debugButton;
    delete newGameButton;
    delete testButton1;
    delete testButton2;
}

bool Toolbox::tileHasMine(int x, int y) const {
    if (!gameState) return false;
    Tile* tile = gameState->getTile(x, y);
    if (!tile) return false;
    return tile->_hasMine;
}

void Toolbox::revealTileForDebug(int x, int y) {
    if (!gameState) return; // Ensure game state exists

    Tile* tile = gameState->getTile(x, y);
    if (!tile) return; // Ensure tile exists

    if (tile->_state == Tile::HIDDEN) {
        if (tile->_hasMine) {
            tile->_state = Tile::EXPLODED;
        } else {
            tile->_state = Tile::REVEALED;
        }
    }
}


void Toolbox::drawDigits(int number, sf::Vector2f position) {

    sf::RenderWindow& window = Toolbox::getInstance().window;

    // Convert the number to a string
    std::string numStr = std::to_string(number);


    float xOffset = 0;

    for (char digitChar : numStr) {
        // Determine digit index in the texture
        int digitIndex;
        if (digitChar == '-') {
            digitIndex = 10;
        } else {
            digitIndex = digitChar - '0';
        }

        // Create digit sprite
        sf::Sprite digitSprite;
        digitSprite.setTexture(digitTexture);

        // Set  texture rectangle
        digitSprite.setTextureRect(sf::IntRect(digitIndex * 21, 0, 21, 32));


        digitSprite.setPosition(position.x + xOffset, position.y);

        // Draw the sprite
        window.draw(digitSprite);


        xOffset += 21;
    }
}


void Toolbox::checkWinCondition() {
    if (!gameState) return;

    bool allNonMinesRevealed = true;

    for (int y = 0; y < gameState->_dimensions.y; ++y) {
        for (int x = 0; x < gameState->_dimensions.x; ++x) {
            Tile* tile = gameState->getTile(x, y);
            if (!tile) continue;

            if (!tile->_hasMine && tile->getState() != Tile::REVEALED) {
                allNonMinesRevealed = false;
                break;
            }
        }
        if (!allNonMinesRevealed) break;
    }

    if (allNonMinesRevealed) {
        gameState->setPlayStatus(GameState::WIN);
        std::cout << "Game won! All non-mine tiles revealed." << std::endl;

        // Update newGameButton texture
        if (newGameButton) {
            newGameButton->getSprite()->setTexture(faceWinTexture);
        }
    }
}
