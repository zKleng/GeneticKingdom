#include "../../include/game/Mercenario.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Mercenario::Mercenario() {
    // Atributos base
    health = 150.f;
    speed = 0.65f;  // intermedio

    resistanceArrow     = 0.4f;  // 40% resistencia
    resistanceMagic     = 0.0f;  // sin resistencia
    resistanceArtillery = 0.4f;  // 40% resistencia

    // Cargar textura (opcional)
    if (!texture.loadFromFile("resources/EnemySprites/HarpiaSprite.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del Ogro.\n";
        // alternativa: usar color sólido en draw()
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Mercenario::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy{ sf::Vector2f(TILE_SIZE, TILE_SIZE) };
    dummy.setFillColor(sf::Color(205, 165, 200)); // algun color xd
    dummy.setPosition(position);
    window.draw(dummy);
}
