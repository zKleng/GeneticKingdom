#include "../../include/game/Mercenario.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Mercenario::Mercenario() {
    // Atributos base
    health = 250.f;
    speed = 0.2f;  // intermedio

    resistanceArrow     = .75f;  // 100% resistencia
    resistanceMagic     = 0.0f;  // sin resistencia
    resistanceArtillery = .75f;  // 100% resistencia

    // Cargar textura (opcional)
    /*if (!texture.loadFromFile("resources/EnemySprites/HarpiaSprite.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del Ogro.\n";
        // alternativa: usar color sólido en draw()
    }*/

    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Mercenario::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy{ sf::Vector2f(TILE_SIZE, TILE_SIZE) };
    dummy.setFillColor(sf::Color(205, 165, 200)); // algun color xd
    dummy.setPosition(position);
    window.draw(dummy);
}

void Mercenario::setGenes(float h, float s, float ra, float rm, float rt) {
    health = h;
    speed = s;
    resistanceArrow = ra;
    resistanceMagic = rm;
    resistanceArtillery = rt;
}
