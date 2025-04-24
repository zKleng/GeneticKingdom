#include <SFML/Graphics.hpp>
#include "../../include/map/Map.h"

constexpr int TILE_SIZE      = 40;
constexpr int TOOLBAR_HEIGHT = TILE_SIZE;
constexpr int CHAR_SIZE      = 16;

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
        sf::VideoMode({
            MAP_WIDTH * TILE_SIZE,
            MAP_HEIGHT * TILE_SIZE + TOOLBAR_HEIGHT
        }),
        "Genetic Kingdom"
    );

    // 1) Carga de la fuente (SFML 3)
    sf::Font font;
    if (!font.openFromFile("resources/arial.ttf")) {
        // No se pudo cargar la fuente
        return -1;
    }

    bool placeEnabled = true;

    while (window.isOpen()) {
        // 2) Manejo de eventos (SFML 3 clásico)
        while (auto maybeEvent = window.pollEvent()) {
            auto& event = *maybeEvent;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto mb = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mpos = sf::Mouse::getPosition(window);

                    // Click en minibarra
                    if (mpos.y < TOOLBAR_HEIGHT && mpos.x < TILE_SIZE) {
                        placeEnabled = !placeEnabled;
                    }
                    // Click en cuadrícula (si está activado el modo)
                    else if (placeEnabled) {
                        int col = mpos.x / TILE_SIZE;
                        int row = (mpos.y - TOOLBAR_HEIGHT) / TILE_SIZE;
                        gameMap.placeTower(row, col);
                    }
                }
            }
        }

        // 3) Dibujado
        window.clear();

        // 3.1) Minibarra de fondo
        sf::RectangleShape toolbar(
            sf::Vector2f(
                float(MAP_WIDTH * TILE_SIZE),
                float(TOOLBAR_HEIGHT)
            )
        );
        toolbar.setPosition(sf::Vector2f(0.f, 0.f));
        toolbar.setFillColor(sf::Color(200, 200, 200));
        window.draw(toolbar);

        // 3.2) Botón toggle
        sf::RectangleShape toggleBtn(
            sf::Vector2f(
                float(TILE_SIZE - 4),
                float(TOOLBAR_HEIGHT - 4)
            )
        );
        toggleBtn.setPosition(sf::Vector2f(2.f, 2.f));
        toggleBtn.setFillColor(placeEnabled ? sf::Color::Green : sf::Color::Red);
        window.draw(toggleBtn);

        // 3.3) Leyenda dinámica
        std::string textStr = placeEnabled
            ? "Colocacion de torres: Activada"
            : "Colocacion de torres: Desactivada";
            sf::Text label(font, textStr, CHAR_SIZE);
        label.setFillColor(sf::Color::Black);
        label.setPosition(sf::Vector2f(
            float(TILE_SIZE + 10),
            float((TOOLBAR_HEIGHT - CHAR_SIZE) / 2)
        ));
        window.draw(label);

        // 3.4) Cuadrícula desplazada hacia abajo
        for (int row = 0; row < MAP_HEIGHT; ++row) {
            for (int col = 0; col < MAP_WIDTH; ++col) {
                sf::RectangleShape cell(
                    sf::Vector2f(
                        float(TILE_SIZE - 2),
                        float(TILE_SIZE - 2)
                    )
                );
                cell.setPosition(sf::Vector2f(
                    float(col * TILE_SIZE),
                    float(TOOLBAR_HEIGHT + row * TILE_SIZE)
                ));
                cell.setFillColor(getColorForTile(
                    gameMap.getTileType(row, col)
                ));
                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}
