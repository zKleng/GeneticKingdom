#pragma once

#include "Enemy.h"
#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>


//clase base de las torres que se usara para el polimorfismo de cada tipo especifico
class Tower{
public:
    Tower();//constructor
    virtual ~Tower() = default;// Destructor virtual para polimorfismo

    // Métodos para la clase torre
    virtual void attackEnemy(std::vector<std::unique_ptr<Enemy>>& enemigos, float deltaTime, int& oro, int& enemigosMuertos) = 0;
    virtual void draw(sf::RenderWindow& window);
    virtual void setPosition(const sf::Vector2f& pos);
    virtual void placeTower();
    virtual bool upgradeTower(int& oro);
    virtual bool canAttack(float deltaTime);
    
    sf::Vector2f getPosition() const;

    void setGridPosition(int r, int c) {
        row = r;
        col = c;
    }
    
    std::pair<int, int> getGridPosition() const {
        return {row, col};
    }

    int getLevel() const { return towerLevel; }

protected:
    // Atributos que tendran las subclases de enemigos
    std::string towerType;
    std::string specialAttack;

    int towerLevel;
    int towerMaxLevel = 4;

    float damage;
    float range;
    float attackReloadTime;
    float specialAttackRechargeTime;
    float probabilidadHabilidad;
    float cost;
    float cooldown;               // tiempo restante antes de poder atacar de nuevo
    float specialCooldown;        // tiempo restante para ataque especial
    float goldPerKill;            // oro base que devuelve por muerte
    float upgradeCosts[3];  // Costo de cada nivel
    
    sf::Clock internalClock;      // reloj para medir tiempos (ataques, especiales)
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    bool hasSprite = false; // Indica si se cargó una textura

    int row = -1;
    int col = -1;
};
