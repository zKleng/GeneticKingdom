#pragma once
#include "Enemy.h"

class Mercenario : public Enemy {
public:
    Mercenario(); // Constructor

    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<Enemy> clone() const override {
        return std::make_unique<Mercenario>(*this);
    }
    
};