//
// Created by arist on 11/29/2024.
//
# include "Tile.h"

#include <iostream>


#include "Toolbox.h"

// Constructs a new tile object at the designated _position.
Tile::Tile(sf::Vector2f position)
    : _position(position),
      _state(HIDDEN),
      _hasMine(false),
      _mineCount(0),
      _isEdge(false),
      _highlighted(false)
{
    _neighbors.fill(nullptr);

    Toolbox& toolbox = Toolbox:: getInstance();

    _sprite.setTexture(toolbox.tileHiddenTexture);


    _sprite.setPosition(_position);

}

// Returns the position of this tile.
sf::Vector2f Tile:: getLocation()
{
    return _position;
}


// Returns current state of this tile.
Tile::State Tile::getState() const {
    return _state; // Return the current state of the tile
}

// Returns pointer to array of Tile pointers
 std::array<Tile*, 8>& Tile :: getNeighbors()
{
    return _neighbors;
}




// Sets the state of this tile. Should trigger other behaviors related to the state change (including visualization).

void Tile::setState(State state) {
    Toolbox& toolbox = Toolbox::getInstance();

    _state = state;

    switch (_state) {
    case HIDDEN:
        _sprite.setTexture(toolbox.tileHiddenTexture);
        break;
    case REVEALED:
        if (_hasMine) {
            _sprite.setTexture(toolbox.mineTexture);
        } else {
            _sprite.setTexture(toolbox.tileRevealedTexture);
        }
        break;
    case FLAGGED:
        _sprite.setTexture(toolbox.flagTexture);
        break;
    case EXPLODED:
        _sprite.setTexture(toolbox.mineTexture);
        _undersprite.setTexture(toolbox.tileRevealedTexture);
        break;
    }
}



// Populates / replaces the neighboring tile container.
 void Tile :: setNeighbors(std::array<Tile*, 8> _neighbors)
{
    this->_neighbors = _neighbors;
}

// Defines the reveal behavior for a tile when the left mouse button is clicked inside its boundaries.
void Tile::onClickLeft() {
    Toolbox& toolbox = Toolbox::getInstance();
    if (toolbox.debugMode) {
        std::cout << "Left click disabled in debug mode." << std::endl;
        return; // Prevent interaction in debug mode
    }

    if (toolbox.gameState->getPlayStatus() == GameState::WIN) {
        std::cout << "Cannot click on the board after winning the game." << std::endl;
        return;
    }

    if (_state != HIDDEN) return; // Only allow interaction with hidden tiles

    if (_hasMine) {
        setState(EXPLODED); //show exploded mine
        toolbox.gameState->setPlayStatus(GameState::LOSS); // Set game status to LOSS

        // Update the newGameButton texture
        if (toolbox.newGameButton) {
            toolbox.newGameButton->getSprite()->setTexture(toolbox.faceLoseTexture);
        }

        std::cout << "Game lost! Mine exploded." << std::endl;

        // enable debug mode
        toolbox.debugMode = true;
        std::cout << "Debug mode turned ON due to loss." << std::endl;

        // Reveal all mines when the player loses
        for (int y = 0; y < toolbox.gameState->_dimensions.y; ++y) {
            for (int x = 0; x < toolbox.gameState->_dimensions.x; ++x) {
                Tile* tile = toolbox.gameState->getTile(x, y);
                if (tile && tile->_hasMine) {
                    tile->setState(Tile::REVEALED);
                }
            }
        }
    } else {
        setState(REVEALED); // Reveal the tile
        if (_mineCount == 0) {
            revealNeighbors();
        }
        std::cout << "Tile revealed, adjacent mines: " << _mineCount << std::endl;
    }

    // Check if the game is won after each click
    toolbox.checkWinCondition();
}




// Toggles this tile’s state between FLAGGED and HIDDEN.
void Tile::onClickRight() {
    Toolbox& toolbox = Toolbox::getInstance();
    if (toolbox.debugMode) {
        std::cout << "Right click disabled in debug mode." << std::endl;
        return; // Prevent interaction in debug mode
    }

    if (toolbox.gameState->getPlayStatus() == GameState::WIN) {
        std::cout << "Cannot click on the board after winning the game." << std::endl;
        return;
    }

    if (_state == HIDDEN) {
        setState(FLAGGED);
        toolbox.mineCounter--;

    } else if (_state == FLAGGED) {
        setState(HIDDEN);
        toolbox.mineCounter++;

    }
    Toolbox::getInstance().checkWinCondition();
}




void Tile::draw() {
    Toolbox& toolbox = Toolbox::getInstance();

    // Update the position of both sprites
    _sprite.setPosition(_position);
    _undersprite.setPosition(_position);

    // Draw the background
    if (_state == EXPLODED) {
        _undersprite.setTexture(toolbox.tileHiddenTexture);
        toolbox.window.draw(_undersprite);
    }

    if (_state == REVEALED) {
        _undersprite.setTexture(toolbox.tileRevealedTexture);
        toolbox.window.draw(_undersprite);
    }

    // Set the main sprite texture
    switch (_state) {
    case HIDDEN:
        _sprite.setTexture(toolbox.tileHiddenTexture);
        break;
    case REVEALED:
        if (_hasMine) {
            _sprite.setTexture(toolbox.mineTexture);
        } else if (_mineCount > 0) {
            _sprite.setTexture(toolbox.numberTextures[_mineCount - 1]);
        } else {
            _sprite.setTexture(toolbox.tileRevealedTexture);
        }
        break;
    case FLAGGED:
        if(!toolbox.debugMode)
        {
            _undersprite.setTexture(toolbox.tileHiddenTexture);
            _sprite.setTexture(toolbox.flagTexture);
        }
        break;
    case EXPLODED:
        _sprite.setTexture(toolbox.mineTexture);
        _undersprite.setTexture(toolbox.tileHiddenTexture);
        break;
    }

    // Draw the primary sprite on top
    toolbox.window.draw(_undersprite);
    toolbox.window.draw(_sprite);
}



// Based on State and mine content of the tile neighbors, set their state to REVEALED.
void Tile::revealNeighbors() {
    for (Tile* neighbor : _neighbors) {
        // Skip null pointers (edges of the board)
        if (!neighbor) {
            continue;
        }


        if (neighbor->getState() == REVEALED || neighbor->getState() == FLAGGED) {
            continue;
        }

        // Reveal neighbor
        neighbor->setState(REVEALED);

        // Render revealed neighbor
        Toolbox::getInstance().window.draw(neighbor->_sprite);

        // recursively reveal its neighbors
        if (neighbor->_mineCount == 0) {
            neighbor->revealNeighbors();
        }
    }
}




