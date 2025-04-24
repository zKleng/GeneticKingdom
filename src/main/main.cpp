#include <SFML/Graphics.hpp>
#include "../../include/map/Map.h"

const int TILE_SIZE = 40;

sf::Color getColorForTile(TileType type) {
    switch (type) {
        case TileType::Entry:   return sf::Color::Green;
        case TileType::Castle:  return sf::Color::Blue;
        case TileType::Tower:   return sf::Color::Red;
        case TileType::Path:    return sf::Color(139, 69, 19);
        default:                return sf::Color::White;
    }
}

int main() {
    Map gameMap;
    sf::RenderWindow window(
      sf::VideoMode({MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE}),
      "Genetic Kingdom"
    );

    while (window.isOpen()) {
        // pollEvent() devuelve std::optional<sf::Event>
        while (auto maybeEvent = window.pollEvent()) {
            auto& event = *maybeEvent;

            // 1) Cerrar ventana
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            // 2) Clic izquierdo
            else if (auto mouseEv = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEv->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mp = sf::Mouse::getPosition(window);
                    int col = mp.x / TILE_SIZE;
                    int row = mp.y / TILE_SIZE;
                    gameMap.placeTower(row, col);
                }
            }
        }

        window.clear();
        for (int r = 0; r < MAP_HEIGHT; ++r) {
            for (int c = 0; c < MAP_WIDTH; ++c) {
                sf::RectangleShape cell({TILE_SIZE - 2.f, TILE_SIZE - 2.f});
                cell.setPosition({float(c * TILE_SIZE),
                                  float(r * TILE_SIZE)});
                cell.setFillColor(getColorForTile(
                    gameMap.getTileType(r, c)
                ));
                window.draw(cell);
            }
        }
        window.display();
    }

    return 0;
}
