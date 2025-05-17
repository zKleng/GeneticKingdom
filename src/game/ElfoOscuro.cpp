#include "../../include/game/ElfoOscuro.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>
#include <iostream>

ElfoOscuro::ElfoOscuro() {
    // Atributos base
    health = 180.f;
    speed = 0.3f;  // Lento

    resistanceArrow     = 0.0f;  // sin resistencia
    resistanceMagic     = 0.8f;  // 80% resistencia
    resistanceArtillery = 0.0f;  // sin resistencia

    // Cargar textura (opcional)
    /*if (!texture.loadFromFile("resources/EnemySprites/ElfoOscuroSprite.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del Ogro.\n";
        // alternativa: usar color sólido en draw()
    }*/

    loadTexture("resources/EnemySprites/ElfoOscuroSprite.png");
    sprite.setPosition(position);
}

void ElfoOscuro::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy{ sf::Vector2f(TILE_SIZE, TILE_SIZE) };
    dummy.setFillColor(sf::Color(20, 20, 30)); // Verde oscuro
    dummy.setPosition(position);
    window.draw(sprite);
}

void ElfoOscuro::setGenes(float h, float s, float ra, float rm, float rt) {
    health = h;
    speed = s;
    resistanceArrow = ra;
    resistanceMagic = rm;
    resistanceArtillery = rt;
}
