#include "../../include/game/Enemy.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

// ---Metodos auxiliares---
//Metodos encargados del dibujado y colocacion del enemigo


// Convierte una celda del mapa (fila, columna) a posición en píxeles
static sf::Vector2f cellToPixel(const sf::Vector2i& cell) {
    return sf::Vector2f(cell.x * TILE_SIZE, TOOLBAR_HEIGHT + cell.y * TILE_SIZE);
}


//Metodos usados para calcular el path que los enemigos van a seguir

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


// Desplazar enemigo a lo largo de la matriz/ventana
void Enemy::moveEnemy() {
    if (path.empty() || pathIndex >= path.size()) return;

    // Mover mientras siga alcanzando targets rápidamente (por velocidad alta)
    while (true) {
        if (pathIndex >= path.size()) return;

        sf::Vector2f target = cellToPixel(path[pathIndex]);
        sf::Vector2f toTarget = target - position;
        float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

        // Si estamos muy cerca del target, pasamos al siguiente nodo del camino
        if (dist < speed) {
            position = target;
            ++pathIndex;
        } else {
            // Movimiento hacia el target actual
            sf::Vector2f dir = normalize(toTarget);
            position += dir * speed;
            break;
        }
    }

    // Actualizar sprite
    sprite.setPosition(position);
}



// Dibujar sprite enemigos
void Enemy::draw(sf::RenderWindow& window) {
    sprite.setPosition(position);
    window.draw(sprite);
}

Enemy::Enemy() : sprite(texture) {
    pathIndex = 0;
}

// recuperar datos de los enemigos
float Enemy::getStats() const {
    return health;
}

// Metodo para gestionar dano segun tipo de ataque
void Enemy::receiveDamage(float amount, std::string tipo) {
    float effectiveDamage = amount;

    if (tipo == "flecha")         effectiveDamage *= (1.f - resistanceArrow);
    else if (tipo == "magia")     effectiveDamage *= (1.f - resistanceMagic);
    else if (tipo == "artilleria")effectiveDamage *= (1.f - resistanceArtillery);

    health -= effectiveDamage;
}


// Metodo para pasarle el path a los enemigos
void Enemy::setPath(const std::vector<sf::Vector2i>& camino) {
    path = camino;
    pathIndex = 0;

    if (!path.empty()) {
        position = cellToPixel(path[0]); // posicionarse al inicio
        sprite.setPosition(position);
    }
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

//Metodos para el algoritmo de genetica

//Obtener los valores base de los enemigos (los genes)
void Enemy::setGenes(float h, float s, float ra, float rm, float rt) {
    health = h;
    speed = s;
    resistanceArrow = ra;
    resistanceMagic = rm;
    resistanceArtillery = rt;
}

// Método para calcular el fitness (aptitud) del enemigo
float Enemy::fitness() const {
    return health * 0.4 + 
    speed * 0.2 + 
    resistanceArrow * 0.1 + 
    resistanceMagic * 0.15 + 
    resistanceArtillery * 0.15;
}