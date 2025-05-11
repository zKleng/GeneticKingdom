#include "../../include/game/Artillero.h"
#include "../../include/game/GameConstants.h"
#include <cmath>
#include <cstdlib> // rand

ArtilleroTower::ArtilleroTower() {
    towerType = "artillero";
    damage = 200.f;
    range = 250.f;
    attackReloadTime = 1.f;
    specialAttackRechargeTime = 4.f;
    cooldown = 0.f;
    specialCooldown = 0.f;
    goldPerKill = 10.f;
    cost = 50;
    probabilidadHabilidad = 0.25f;

    upgradeCosts[0] = 30.f;
    upgradeCosts[1] = 60.f;
    upgradeCosts[2] = 90.f;
}

void ArtilleroTower::attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) {
    // Actualizar cooldowns
    cooldown = std::max(0.f, cooldown - deltaTime);
    specialCooldown = std::max(0.f, specialCooldown - deltaTime);

    if (cooldown > 0.f)
        return;

        for (auto it = enemigos.begin(); it != enemigos.end(); ++it) {
            Enemy* e = it->get();
    
            // Calcular distancia entre torre y enemigo
            float dx = e->getPosition().x - position.x;
            float dy = e->getPosition().y - position.y;
            float dist = std::sqrt(dx * dx + dy * dy);
    
            if (dist <= range) {
                // Calcular daño
                float dañoFinal = damage;
    
                if (specialCooldown <= 0.f && (rand() % 100) < static_cast<int>(probabilidadHabilidad * 100)) {
                    dañoFinal *= 1.5f;
                    specialCooldown = specialAttackRechargeTime;
                }
    
                // Aplicar daño
                std::cout << "[Torre artillera] Ataco con artilleria al enemigo en posición: (" 
                          << static_cast<int>(e->getPosition().x) << ", "
                          << static_cast<int>(e->getPosition().y) << ")\n";
    
                e->receiveDamage(dañoFinal, "artilleria");
    
                // Si muere, eliminarlo
                if (e->getStats() <= 0.f) {
                    oro += static_cast<int>(goldPerKill);
                    enemigosMuertos++;
                    enemigos.erase(it);
                }
    
                cooldown = attackReloadTime;  // reiniciar cooldown tras atacar
                break; // atacar solo a uno
            }
        }
}

bool ArtilleroTower::upgradeTower(int& oro) {
    if (towerLevel >= towerMaxLevel) return false;

    float cost = upgradeCosts[towerLevel - 1];
    if (oro >= cost) {
        oro -= cost;
        ++towerLevel;

        switch (towerLevel) {
            case 2:
                damage += 75.f;
                range += 15.f;
                break;
            case 3:
                damage += 100.f;
                range += 20.f;
                break;
            case 4:
            damage += 150.f;
            range += 25.f;
            break;
        }

        return true;
    }

    return false;
}

void ArtilleroTower::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
    
    dummy.setPosition(position);
    
    // Cambiar color según el nivel
    if (towerLevel == 1)
        dummy.setFillColor(sf::Color(139, 69, 19));  // café oscuro (nivel base)
    else if (towerLevel == 2)
        dummy.setFillColor(sf::Color(255, 165, 0));  // naranja (mejorado 1 vez)
    else if (towerLevel == 3)
        dummy.setFillColor(sf::Color::Red);          // rojo (mejorado 2 veces)
    else if (towerLevel == 4)
        dummy.setFillColor(sf::Color::Magenta);      // magenta (mejorado 3 veces)
                    
    
    window.draw(dummy);
}