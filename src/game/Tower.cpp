#include "../../include/game/Tower.h"
#include "../../include/game/GameConstants.h"

Tower::Tower() 
    : towerLevel(1),
      cooldown(0.f),
      specialCooldown(0.f),
      position(0.f, 0.f), 
      texture(), 
      sprite(texture), 
      hasSprite(false)
{
    // Valores por defecto para cada subclase
    damage = 0.f;
    range = 0.f;
    attackReloadTime = 1.f;
    specialAttackRechargeTime = 5.f;
    cost = 0.f;
    goldPerKill = 0.f;

    // Inicializar los costos de mejora a 0 por defecto
    for (int i = 0; i < 4; ++i) {
        upgradeCosts[i] = 0.f;
    }
}

void Tower::draw(sf::RenderWindow& window) {
    if (hasSprite) {
        sprite.setPosition(position);
        window.draw(sprite);
        std::cout << "[DRAW] Sprite en: " << position.x << "," << position.y << "\n";
    } else {
        sf::RectangleShape dummy{ sf::Vector2f(float(TILE_SIZE), float(TILE_SIZE)) };
        dummy.setPosition(position);
        dummy.setFillColor(sf::Color::Red); // fallback visible
        window.draw(dummy);
        std::cout << "[DRAW] Dibujo fallback\n";
    }
}
void Tower::placeTower() {
    if (texture.getSize().x > 0 && texture.getSize().y > 0) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        hasSprite = true;
    } else {
        hasSprite = false;
    }
}

bool Tower::upgradeTower(int& oro) {
    if (towerLevel >= towerMaxLevel)
        return false;

    float upgradeCost = upgradeCosts[towerLevel - 1];
    if (oro >= upgradeCost) {
        oro -= upgradeCost;
        ++towerLevel;
        damage *= 1.25f; // Ejemplo de incremento de daño
        range += 5.f;    // Ejemplo de incremento de alcance
        return true;
    }

    return false;
}

bool Tower::canAttack(float deltaTime) {
    cooldown -= deltaTime;
    specialCooldown -= deltaTime;

    if (cooldown <= 0.f) {
        cooldown = attackReloadTime;
        return true;
    }

    return false;
}

void Tower::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

sf::Vector2f Tower::getPosition() const {
    return position;
}


