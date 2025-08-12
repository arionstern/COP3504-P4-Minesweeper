#include <iostream>
#include "minesweeper.h"
#include <SFML/Graphics.hpp>
#include "Toolbox.h"

// Resets all states/objects and generates a default game state.
void restart() {
    Toolbox& toolbox = Toolbox::getInstance();
    std::cout << "Restarting game..." << std::endl;

    if (toolbox.gameState) {
        delete toolbox.gameState;
    }

    try {
        // Create new game state
        toolbox.gameState = new GameState(sf::Vector2i(25, 16), 50);
        std::cout << "New game state initialized." << std::endl;

        // Initialize mine counter
        toolbox.initializeMineCounter(toolbox.gameState->getMineCount());

    } catch (std::exception& e) {
        std::cerr << "Error initializing game state: " << e.what() << std::endl;
        return;
    }

    toolbox.debugMode = false;
    std::cout << "Debug mode turned off." << std::endl;
    if (toolbox.newGameButton) {
        toolbox.newGameButton->getSprite()->setTexture(toolbox.faceNeutralTexture);
    }

}


// Toggles debug mode on/off.
void toggleDebugMode() {
    Toolbox& toolbox = Toolbox::getInstance();
    toolbox.debugMode = !toolbox.debugMode;
    std::cout << "Debug mode " << (toolbox.debugMode ? "ON" : "OFF") << std::endl;

    GameState* gameState = toolbox.gameState;
    if (!gameState) return;

    for (int y = 0; y < gameState->_dimensions.y; ++y) {
        for (int x = 0; x < gameState->_dimensions.x; ++x) {
            Tile* tile = gameState->getTile(x, y);
            if (!tile) continue;

            if (toolbox.debugMode) {
                // If the tile is flagged and has a mine, show it as a mine
                if (tile->_hasMine && tile->getState() == Tile::FLAGGED) {
                    tile->_sprite.setTexture(toolbox.mineTexture);
                }
                // If the tile is hidden and has a mine, reveal it
                else if (tile->_hasMine && tile->getState() == Tile::HIDDEN) {
                    tile->setState(Tile::REVEALED);
                }
            } else {
                // Revert flagged tiles with mines back to the flag texture
                if (tile->_hasMine && tile->getState() == Tile::FLAGGED) {
                    tile->_sprite.setTexture(toolbox.flagTexture);
                }
                // Hide previously revealed mines when debug mode is off
                else if (tile->_hasMine && tile->getState() == Tile::REVEALED) {
                    tile->setState(Tile::HIDDEN);
                }

            }
        }
    }
}



// Returns true if debug mode is active.
bool getDebugMode() {
    return Toolbox::getInstance().debugMode;
}

// Draws all UI elements according to the current game state and debug mode.
void render() {
    Toolbox& toolbox = Toolbox::getInstance();
    toolbox.window.clear();

    for (int y = 0; ; ++y) {
        Tile* rowCheck = toolbox.gameState->getTile(0, y);
        if (!rowCheck) break;

        for (int x = 0; ; ++x) {
            Tile* tile = toolbox.gameState->getTile(x, y);
            if (!tile) break;

            tile->draw();

            // Debug mode: Reveal mines
            if (getDebugMode() && toolbox.tileHasMine(x, y)) {
                toolbox.revealTileForDebug(x, y);
                tile->draw();
            }
        }
    }

    // Render buttons
    if (toolbox.debugButton) toolbox.window.draw(*toolbox.debugButton->getSprite());
    if (toolbox.newGameButton) toolbox.window.draw(*toolbox.newGameButton->getSprite());
    if (toolbox.testButton1) toolbox.window.draw(*toolbox.testButton1->getSprite());
    if (toolbox.testButton2) toolbox.window.draw(*toolbox.testButton2->getSprite());
    if (toolbox.testButton3) {
        toolbox.window.draw(*toolbox.testButton3->getSprite());
    }


    toolbox.drawDigits(toolbox.mineCounter, sf::Vector2f(0, 514));
    toolbox.window.display();
}




// Handles event-polling, rendering, and other logic.
int gameLoop() {
    Toolbox& toolbox = Toolbox::getInstance();


    if (!toolbox.gameState) {
        std::cerr << "Error: gameState is not initialized." << std::endl;
        return -1;
    }

    // Main game loop
    while (toolbox.window.isOpen()) {
        sf::Event event;

        // Polling events
        while (toolbox.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "Window close event triggered." << std::endl;
                toolbox.window.close();
                return 0;
            }


            // Mouse button pressed
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                std::cout << "Mouse clicked at position: " << mousePos.x << ", " << mousePos.y << std::endl;

                // Check buttons
                if (toolbox.debugButton && toolbox.debugButton->getSprite()->getGlobalBounds().contains(mousePos)) {
                    toolbox.debugButton->onClick();
                    std::cout << "Debug button clicked." << std::endl;
                } else if (toolbox.newGameButton && toolbox.newGameButton->getSprite()->getGlobalBounds().contains(mousePos)) {
                    toolbox.newGameButton->onClick();
                    toolbox.newGameButton->getSprite()->setTexture(toolbox.faceNeutralTexture);
                    std::cout << "Button texture updated to neutral face." << std::endl;
                    std::cout << "New game button clicked." << std::endl;

                    toolbox.initializeMineCounter(toolbox.gameState->getMineCount());


                } else if (toolbox.testButton1 && toolbox.testButton1->getSprite()->getGlobalBounds().contains(mousePos)) {
                    toolbox.testButton1->onClick();
                    // Initialize the mine counter after creating a new game state
                    toolbox.initializeMineCounter(toolbox.gameState->getMineCount());
                    toolbox.newGameButton->getSprite()->setTexture(toolbox.faceNeutralTexture);
                    std::cout << "Button texture updated to neutral face." << std::endl;
                    std::cout << "Test board 1 button clicked." << std::endl;


                } else if (toolbox.testButton2 && toolbox.testButton2->getSprite()->getGlobalBounds().contains(mousePos)) {
                    toolbox.testButton2->onClick();
                    toolbox.newGameButton->getSprite()->setTexture(toolbox.faceNeutralTexture);
                    std::cout << "Button texture updated to neutral face." << std::endl;
                    std::cout << "Test board 2 button clicked." << std::endl;

                    toolbox.initializeMineCounter(toolbox.gameState->getMineCount());

                }
                else if (toolbox.testButton3 && toolbox.testButton3->getSprite()->getGlobalBounds().contains(mousePos)) {
                    toolbox.testButton3->onClick();
                    toolbox.newGameButton->getSprite()->setTexture(toolbox.faceNeutralTexture);
                    std::cout << "Test board 3 button clicked." << std::endl;
                    toolbox.initializeMineCounter(toolbox.gameState->getMineCount());
                }
                else {
                    // Check tiles
                    for (int y = 0; ; ++y) {
                        Tile* rowCheck = toolbox.gameState->getTile(0, y);
                        if (!rowCheck) {
                            break;
                        }

                        for (int x = 0; ; ++x) {
                            Tile* tile = toolbox.gameState->getTile(x, y);
                            if (!tile) {
                                break;
                            }

                            // Check if the tile contains the click
                            sf::Vector2f tilePos = tile->getLocation();
                            sf::FloatRect bounds(tilePos.x, tilePos.y, 32, 32);
                            if (bounds.contains(mousePos)) {
                                std::cout << "Tile clicked at (" << x << ", " << y << ")" << std::endl;
                                if (event.mouseButton.button == sf::Mouse::Left) {
                                    tile->onClickLeft();
                                    std::cout << "Left click action on tile." << std::endl;
                                } else if (event.mouseButton.button == sf::Mouse::Right) {
                                    tile->onClickRight();
                                    std::cout << "Right click action on tile." << std::endl;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Render the game state
        try {
            render();
        } catch (std::exception& e) {
            std::cerr << "Error during render: " << e.what() << std::endl;
            return -1;
        }
    }

    return 0;
}

void updateNewGameButtonTexture() {
    Toolbox& toolbox = Toolbox::getInstance();

    if (!toolbox.newGameButton || !toolbox.gameState) return; // Safety checks

    sf::Texture* newTexture = nullptr;

    switch (toolbox.gameState->getPlayStatus()) {
    case GameState::WIN:
        newTexture = &toolbox.faceWinTexture;
        break;
    case GameState::LOSS:
        newTexture = &toolbox.faceLoseTexture;

        break;
    default:
        newTexture = &toolbox.faceNeutralTexture;
        break;
    }

    if (newTexture) {
        toolbox.newGameButton->getSprite()->setTexture(*newTexture);
    }
}




// Launches the game.
int launch() {
    restart();
    return gameLoop();
}

// Main entry point
int main() {
    return launch();
}
