//
// Created by arist on 11/29/2024.
//


#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>

class Button {
private:
    sf::Vector2f _position;

    sf::Sprite* _sprite;
    std::function<void(void)> _onClick; // Callback function for button click

public:
    // Constructor
    Button(sf::Vector2f _position, std::function<void(void)> _onClick);


    // Getters


    sf::Vector2f getPosition();
    sf::Sprite* getSprite();

    // Setters
    void setSprite(sf::Sprite* sprite);

    // Click Handler
    void onClick();
};

#endif // BUTTON_H
