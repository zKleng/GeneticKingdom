#pragma once

#include "Tower.h"

class MagoTower : public Tower {
public:
    MagoTower();

    void attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) override;
    void draw(sf::RenderWindow& window) override;
    virtual bool upgradeTower(int& oro) override;
    std::string getTexture(int level) override;
};