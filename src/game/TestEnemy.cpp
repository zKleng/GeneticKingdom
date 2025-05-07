#include "../../include/game/TestEnemy.h"
#include <SFML/Graphics.hpp>

TestEnemy::TestEnemy() {
    health = 100;
    speed = 1.0f;

    // Cargá una textura simple o dejalo vacío
    if (!texture.loadFromFile("resources/EnemySprites/test.png")) {
        std::cerr << "[ERROR] No se pudo cargar la textura del TestEnemy.\n";
    }
    sprite.setTexture(texture);
}

void TestEnemy::draw(sf::RenderWindow& window) {
    sf::RectangleShape dummy(sf::Vector2f(40.f, 40.f));
    dummy.setFillColor(sf::Color::Magenta);
    dummy.setPosition(position);
    window.draw(dummy);
}
