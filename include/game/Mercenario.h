#pragma once
#include "Enemy.h"

class Mercenario : public Enemy {
public:
    Mercenario(); // Constructor

    void draw(sf::RenderWindow& window) override;
};