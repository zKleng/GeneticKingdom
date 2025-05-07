#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class TestEnemy : public Enemy {
public:
    TestEnemy();

    void draw(sf::RenderWindow& window) override;
};
