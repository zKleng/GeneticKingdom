#pragma once
#include "Enemy.h"

class ElfoOscuro : public Enemy {
public:
    ElfoOscuro(); // Constructor

    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<Enemy> clone() const override {
        return std::make_unique<ElfoOscuro>(*this);
    }
    void setGenes(float h, float s, float ra, float rm, float rt) override;
    
};