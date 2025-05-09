#pragma once
#include "Enemy.h"

class Ogro : public Enemy {
public:
    Ogro(); // Constructor

    void draw(sf::RenderWindow& window) override;
};
