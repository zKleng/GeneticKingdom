#include <SFML/Graphics.hpp>
#include "../../include/map/Map.h"

const int TILE_SIZE = 40;

sf::Color getColorForTile(TileType type) {
    switch (type) {
        case TileType::Entry:   return sf::Color::Green;
        case TileType::Castle:  return sf::Color::Blue;
        case TileType::Tower:   return sf::Color::Red;
        case TileType::Path:    return sf::Color(128, 128, 128);
        default:                return sf::Color::White;
    }
}

int main() {
    Map gameMap;

    // Coloca torres para verlas
    gameMap.placeTower(3, 3);
    gameMap.placeTower(4, 5);
    gameMap.placeTower(7, 10);

    // Usar sf::Vector2u para VideoMode
    sf::RenderWindow window(sf::VideoMode({MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE}), "Genetic Kingdom");

    while (window.isOpen()) {
        // Nuevo sistema: pollEvent() devuelve std::optional<sf::Event>
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        for (int row = 0; row < MAP_HEIGHT; ++row) {
            for (int col = 0; col < MAP_WIDTH; ++col) {
                sf::RectangleShape rect(sf::Vector2f(TILE_SIZE - 2.0f, TILE_SIZE - 2.0f));
                rect.setPosition(sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE));
                rect.setFillColor(getColorForTile(gameMap.getTileType(row, col)));
                window.draw(rect);
            }
        }

        window.display();
    }

    return 0;
}
