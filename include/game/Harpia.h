#pragma once
#include "Enemy.h"

class Harpia : public Enemy {
public:
    Harpia(); // Constructor

    void draw(sf::RenderWindow& window) override;
};