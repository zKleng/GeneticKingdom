#pragma once
#include "Enemy.h"

class ElfoOscuro : public Enemy {
public:
    ElfoOscuro(); // Constructor

    void draw(sf::RenderWindow& window) override;
};