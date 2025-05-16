#pragma once
#include "Enemy.h"

class Ogro : public Enemy {
public:
    Ogro(); // Constructor

    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<Enemy> clone() const override {
        return std::make_unique<Ogro>(*this);
    }
    void setGenes(float h, float s, float ra, float rm, float rt) override;

    
};
