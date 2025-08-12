//
// Created by arist on 11/29/2024.
//

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <array>

class Tile {
    friend class GameState;
    friend class Toolbox;
    friend void toggleDebugMode();

public:
    // Enumeration for tile state
    enum State { HIDDEN, REVEALED, FLAGGED, EXPLODED };

private:
    // Attributes
    sf::Vector2f _position;       // Position of tile
    State _state;                 // state of tile
    bool _hasMine;
    int _mineCount;               // Number mines adjacent to tile
    std::array<Tile*, 8> _neighbors; // Pointers to neighboring tiles


    sf::Sprite _sprite;
    sf::Sprite _undersprite;

    bool _isEdge;
    bool _highlighted;
    sf::RenderWindow* _window;    // Pointer to  render window

    friend class GameState;
public:
    // Constructor
    Tile(sf::Vector2f position);

    // Getters and Setters
    sf::Vector2f getLocation();
    State getState() const;
    std::array<Tile*, 8>& getNeighbors() ;




    void setState(State _state);
    void setNeighbors(std::array<Tile*, 8> _neighbors);


    void onClickLeft();
    void onClickRight();
    void draw();

protected:
    void revealNeighbors();

};

#endif //TILE_H
