//
// Created by arist on 12/1/2024.
//

#include "Button.h"

// Constructs a new object at the specified _position which invokes the _onClick callback when clicked.

Button::Button(sf::Vector2f position, std::function<void(void)> onClick)
    : _position(position), _onClick(onClick), _sprite(nullptr) {

}

// Returns the position of the button.
sf::Vector2f Button::getPosition()  {
    return _position;
}

// Returns the current sprite of the button.
sf::Sprite* Button::getSprite()  {
    return _sprite;
}



// Sets this button’s visualization to the specified _sprite.
void Button::setSprite(sf::Sprite* sprite) {
    _sprite = sprite;
    if (_sprite) {
        _sprite->setPosition(_position);
    }
}

void Button::onClick() {
    if (_onClick) {
        _onClick();
    }
}






