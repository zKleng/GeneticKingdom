#include "../../include/game/Artillero.h"
#include "../../include/game/GameConstants.h"
#include <cmath>
#include <cstdlib> // rand

ArtilleroTower::ArtilleroTower() {
    towerType = "artillero";
    damage = 200.f;
    range = 250.f;
    attackReloadTime = 1.f;
    specialAttackRechargeTime = 8.f;
    cooldown = 0.f;
    specialCooldown = 0.f;
    goldPerKill = 5.f;
    cost = 50;
    probabilidadHabilidad = 0.25f;

    upgradeCosts[0] = 30.f;
    upgradeCosts[1] = 50.f;
    upgradeCosts[2] = 80.f;
}

void ArtilleroTower::attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) {
    // Actualizar cooldowns
    cooldown = std::max(0.f, cooldown - deltaTime);
    specialCooldown = std::max(0.f, specialCooldown - deltaTime);

    if (cooldown > 0.f)
        return;

    bool attacked = false;


    auto it = enemigos.begin();
    while (it != enemigos.end()) {
        Enemy* e = it->get();
        float dx = e->getPosition().x - position.x;
        float dy = e->getPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range) {
            float dañoFinal = damage;

            if (specialCooldown <= 0.f && (rand() % 100) < 20) {
                dañoFinal *= 1.5f;
                specialCooldown = specialAttackRechargeTime;
            }

            std::cout << "[Torre Artillero] Daño aplicado: " << dañoFinal
                      << " al enemigo en posición: (" 
                      << static_cast<int>(e->getPosition().x) << ", "
                      << static_cast<int>(e->getPosition().y) << ")\n";

            e->receiveDamage(dañoFinal, "flecha");
            attacked = true;

            if (e->getStats() <= 0.f) {
                oro += static_cast<int>(goldPerKill);
                enemigosMuertos++;
                it = enemigos.erase(it);// eliminar el enemigo
                continue; 
            }
        }

        ++it;
    }

    if (attacked) {
        cooldown = attackReloadTime; // resetear cooldown de ataque
    }
}

void ArtilleroTower::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
    dummy.setFillColor(sf::Color::Green);
    dummy.setPosition(position);
    window.draw(dummy);
}