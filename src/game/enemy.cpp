#include "../../include/game/Enemy.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>


Enemy::Enemy() : sprite(texture) {
    pathIndex = 0;
}

// Utilidad para calcular la distancia entre dos puntos
static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Utilidad para normalizar un vector
static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    return (len != 0.f) ? sf::Vector2f(v.x / len, v.y / len) : sf::Vector2f(0.f, 0.f);
}

// Convierte una celda del mapa (fila, columna) a posición en píxeles
static sf::Vector2f cellToPixel(const sf::Vector2i& cell) {
    return sf::Vector2f(cell.x * TILE_SIZE, TOOLBAR_HEIGHT + cell.y * TILE_SIZE);
}

void Enemy::moveEnemy() {
    if (path.empty()) return;
    if (pathIndex >= path.size()) return;

    sf::Vector2f target = cellToPixel(path[pathIndex]);

    if (distance(position, target) < 1.f) {
        ++pathIndex;
    } else {
        sf::Vector2f dir = normalize(target - position);
        position += dir * speed;
        sprite.setPosition(position);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    sprite.setPosition(position);
    window.draw(sprite);
}

float Enemy::getStats() const {
    return health;
}

void Enemy::receiveDamage(float amount, std::string tipo) {
    float effectiveDamage = amount;

    if (tipo == "flecha")         effectiveDamage *= (1.f - resistanceArrow);
    else if (tipo == "magia")     effectiveDamage *= (1.f - resistanceMagic);
    else if (tipo == "artilleria")effectiveDamage *= (1.f - resistanceArtillery);

    health -= effectiveDamage;
}

void Enemy::setPath(const std::vector<sf::Vector2i>& camino) {
    path = camino;
    pathIndex = 0;

    if (!path.empty()) {
        position = cellToPixel(path[0]); // posicionarse al inicio
        sprite.setPosition(position);
    }
}