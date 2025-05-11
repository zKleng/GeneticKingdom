#include "../../include/game/Ogro.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Ogro::Ogro() {
    // Atributos base
    health = 150.f;
    speed = 0.15f;  // Lento

    resistanceArrow     = 0.4f;  // 40% resistencia
    resistanceMagic     = 0.0f;  // sin resistencia
    resistanceArtillery = 0.0f;  // sin resistencia

    // Cargar textura (opcional)
    if (!texture.loadFromFile("resources/EnemySprites/OgroSprite.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del Ogro.\n";
        // alternativa: usar color sólido en draw()
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Ogro::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy{ sf::Vector2f(TILE_SIZE, TILE_SIZE) };
    dummy.setFillColor(sf::Color(80, 180, 80)); // Verde oscuro
    dummy.setPosition(position);
    window.draw(dummy);
}
