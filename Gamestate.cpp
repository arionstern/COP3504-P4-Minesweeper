//
// Created by arist on 11/30/2024.
//
#include "GameState.h"
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <numeric>   // For std::iota
#include <random>    // For std::default_random_engine
#include <algorithm> // For std::shuffle
#include "GameState.h"
#include <fstream>
#include <iostream>
#include <stdexcept> // For exceptions

// Constructs a new random game state with specified tile _dimensions and the specified _numberOfMines.
GameState::GameState(sf::Vector2i dimensions, int numberOfMines)
    : _dimensions(dimensions), _numberOfMines(numberOfMines), _flagCount(0), _status(PLAYING) {
    // Initialize random seed
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create the board
    _board.resize(_dimensions.y);
    for (int y = 0; y < _dimensions.y; ++y) {
        _board[y].reserve(_dimensions.x);
        for (int x = 0; x < _dimensions.x; ++x) {
            _board[y].emplace_back(sf::Vector2f(x * 32, y * 32));
        }
    }

    // Randomly place mines
    std::vector<int> minePositions(_dimensions.x * _dimensions.y);
    std::iota(minePositions.begin(), minePositions.end(), 0);
    std::shuffle(minePositions.begin(), minePositions.end(), gen);

    for (int i = 0; i < _numberOfMines; ++i) {
        int index = minePositions[i];
        int x = index % _dimensions.x;
        int y = index / _dimensions.x;
        _board[y][x]._hasMine = true;
    }

    // Calculate adjacent mine counts
    const std::array<std::pair<int, int>, 8> offsets = {{
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    }};

    for (int y = 0; y < _dimensions.y; ++y) {
        for (int x = 0; x < _dimensions.x; ++x) {
            if (_board[y][x]._hasMine) continue; // Skip tiles with mines

            int mineCount = 0;
            for (const auto& [dx, dy] : offsets) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < _dimensions.x && ny >= 0 && ny < _dimensions.y) {
                    if (_board[ny][nx]._hasMine) {
                        ++mineCount;
                    }
                }
            }
            _board[y][x]._mineCount = mineCount; // Directly set _mineCount
        }
    }

    for (int y = 0; y < _dimensions.y; ++y)
    {
        for (int x = 0; x < _dimensions.x; ++x) {
            Tile* currentTile = getTile(x, y);
            if (!currentTile) continue;

            std::array<Tile*, 8> neighbors = {nullptr};
            const std::array<std::pair<int, int>, 8> offsets = {{
                {-1, -1}, {0, -1}, {1, -1},
                {-1,  0},          {1,  0},
                {-1,  1}, {0,  1}, {1,  1}
            }};

            for (size_t i = 0; i < offsets.size(); ++i) {
                int nx = x + offsets[i].first;
                int ny = y + offsets[i].second;

                if (nx >= 0 && nx < _dimensions.x && ny >= 0 && ny < _dimensions.y) {
                    neighbors[i] = getTile(nx, ny);
                }
            }

            currentTile->setNeighbors(neighbors);
        }
    }
}


GameState::GameState(const char* filepath) : _flagCount(0), _status(PLAYING), _numberOfMines(0) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + std::string(filepath));
    }


    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    int rows = lines.size();
    int cols = rows > 0 ? lines[0].size() : 0;

    std::cout << "Parsed dimensions: " << rows << "x" << cols << std::endl;

    const int MAX_SIZE = 1000;
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        throw std::runtime_error("Invalid board dimensions.");
    }

    _dimensions = sf::Vector2i(cols, rows);

    // Initialize the board dynamically
    _board.resize(rows);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            _board[y].emplace_back(sf::Vector2f(x * 32, y * 32));
            if (lines[y][x] == '1') {
                _board[y][x]._hasMine = true; // Set mine if '1'
                ++_numberOfMines;
            } else if (lines[y][x] != '0') {
                throw std::runtime_error("Invalid character in board file: " + std::string(1, lines[y][x]));
            }
        }
    }



    // Assign neighbors for each tile
    const std::array<std::pair<int, int>, 8> offsets = {{
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    }};

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (_board[y][x]._hasMine) continue; // Skip tiles with mines

            int mineCount = 0;
            for (const auto& [dx, dy] : offsets) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows && _board[ny][nx]._hasMine) {
                    ++mineCount;
                }
            }
            _board[y][x]._mineCount = mineCount;
            std::cout << "Tile at (" << x << ", " << y << ") has " << mineCount << " adjacent mines." << std::endl;
        }
    }

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            std::array<Tile*, 8> neighbors = {nullptr}; // Initialize neighbor array

            for (size_t i = 0; i < offsets.size(); ++i) {
                int nx = x + offsets[i].first;
                int ny = y + offsets[i].second;


                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                    neighbors[i] = &_board[ny][nx]; // Set valid neighbors
                }
            }

            _board[y][x].setNeighbors(neighbors); // Assign neighbors to the tile
        }
    }

    std::cout << "Number of mines: " << _numberOfMines << std::endl;
    std::cout << "Board initialized successfully." << std::endl;
}












// Current count of the number of flags placed on the screen.
int GameState::getFlagCount()  {
    return _flagCount;
}

// Current count of the number of mines actually on the board.
int GameState::getMineCount()  {
    return _numberOfMines;
}

// Returns a pointer to the Tile at the specified coordinates, or nullptr if out of bounds.
Tile* GameState::getTile(int x, int y) {
    if (x < 0 || x >= _dimensions.x || y < 0 || y >= _dimensions.y) {
        return nullptr;
    }

    if (y >= _board.size() || x >= _board[y].size()) {
        return nullptr;
    }

    return &_board[y][x];
}

// Returns the play status of the game.
GameState::PlayStatus GameState::getPlayStatus()  {
    return _status;
}

// Sets the play status of the game.
void GameState::setPlayStatus(PlayStatus status) {
    _status = status;



}