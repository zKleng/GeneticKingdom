#include "../../include/game/Harpia.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Harpia::Harpia() {
    // Atributos base
    health = 150.f;
    speed = 0.25f;  // intermedio

    resistanceArrow     = 0.0f;  // sin resistencia
    resistanceMagic     = 0.0f;  // sin resistencia
    resistanceArtillery = 1.0f;  // 100% resistencia

    // Cargar textura (opcional)
    if (!texture.loadFromFile("resources/EnemySprites/HarpiaSprite.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del Ogro.\n";
        // alternativa: usar color sólido en draw()
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Harpia::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy{ sf::Vector2f(TILE_SIZE, TILE_SIZE) };
    dummy.setFillColor(sf::Color(255, 165, 0)); // Naranja
    dummy.setPosition(position);
    window.draw(dummy);
}

void Harpia::setGenes(float h, float s, float ra, float rm, float rt) {
    health = h;
    speed = s;
    resistanceArrow = ra;
    resistanceMagic = rm;
    resistanceArtillery = rt;
}
