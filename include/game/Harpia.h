#pragma once
#include "Enemy.h"

class Harpia : public Enemy {
public:
    Harpia(); // Constructor

    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<Enemy> clone() const override {
        return std::make_unique<Harpia>(*this);
    }
    
};