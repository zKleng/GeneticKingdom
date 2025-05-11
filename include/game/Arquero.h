#pragma once

#include "Tower.h"

class ArqueroTower : public Tower {
public:
    ArqueroTower();

    void attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) override;
    void draw(sf::RenderWindow& window) override;
    virtual bool upgradeTower(int& oro) override;
};