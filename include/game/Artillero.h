#pragma once

#include "Tower.h"

class ArtilleroTower : public Tower {
public:
    ArtilleroTower();

    void attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) override;
    void draw(sf::RenderWindow& window) override;

};