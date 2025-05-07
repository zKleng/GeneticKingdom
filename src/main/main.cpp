#include "../../include/map/Map.h"
#include "../../include/game/TestEnemy.h"
#include "../../include/game/GameConstants.h"

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <array>
#include <algorithm>
#include <iostream>   // para mensajes de fallo


enum class Phase  { Construction, Wave };
enum class Action { Place, Upgrade };

static std::string formatTime(float seconds) {
    int s = std::max(int(seconds), 0);
    int m = s / 60;  s %= 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << m
        << ":" << std::setw(1) << std::setfill('0') << s;
    return oss.str();
}

static sf::Color getColorForTile(TileType type) {
    switch (type) {
        case TileType::Entry:   return sf::Color::Green;
        case TileType::Castle:  return sf::Color::Blue;
        case TileType::Tower1:  return sf::Color::Red;
        case TileType::Tower2:  return sf::Color::Blue;
        case TileType::Tower3:  return sf::Color::Green;
        case TileType::Path:    return sf::Color(139, 69, 19);
        default:                return sf::Color::White;
    }
}

// Array de los tipos de torre para los botones 0,1,2
static constexpr std::array<TileType,3> towerButtons = {
    TileType::Tower1,
    TileType::Tower2,
    TileType::Tower3
};

int main() {
    Map gameMap;

    // Enemigos activos
    std::vector<std::unique_ptr<Enemy>> enemigos;

    // Ventana
    std::uint32_t winW = static_cast<std::uint32_t>(MAP_WIDTH  * TILE_SIZE + PANEL_WIDTH);
    std::uint32_t winH = static_cast<std::uint32_t>(MAP_HEIGHT * TILE_SIZE + TOOLBAR_HEIGHT);
    sf::RenderWindow window(sf::VideoMode({ winW, winH }), "Genetic Kingdom");

    // Fuente
    sf::Font font;
    if (!font.openFromFile("resources/Fonts/arial.ttf"))
        return -1;

    // Estado
    Phase      phase         = Phase::Construction;
    Action     action        = Action::Place;
    TileType   selectedTower = TileType::Tower1;
    sf::Clock  phaseClock;
    int        oro           = 1000;
    int        placedCount   = 0;
    int        upgradeCount  = 0;
    int        generaciones  = 0;
    int        enemigosMuertos = 0;
    float      fitnessActual   = 0.f;
    float      probMutacion    = 0.05f;
    int        mutacionesAcumuladas = 0;

    while (window.isOpen()) {
        // Eventos
        while (auto mev = window.pollEvent()) {
            if (mev->is<sf::Event::Closed>()) {
                window.close();
                break;
            }
            if (auto mb = mev->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mpos = sf::Mouse::getPosition(window);

                // 1) Clic en toolbar?
                if (mpos.y < int(TOOLBAR_HEIGHT)) {
                    int idx = mpos.x / TILE_SIZE;  // 0..3
                    if (idx >= 0 && idx <= 3) {
                        if (idx < 3) {
                            action        = Action::Place;
                            selectedTower = towerButtons[idx];
                        } else {
                            action = Action::Upgrade;
                        }
                    }
                    continue;
                }

                // 2) Clic en mapa
                int col = mpos.x / TILE_SIZE;
                int row = (mpos.y - TOOLBAR_HEIGHT) / TILE_SIZE;
                if (row>=0 && row<MAP_HEIGHT && col>=0 && col<MAP_WIDTH) {
                    if (mb->button == sf::Mouse::Button::Left) {
                        if (action == Action::Place) {
                            if (gameMap.placeTower(row, col, selectedTower, oro)) {
                                ++placedCount;
                            } else {
                                std::cout << "Cannot place tower at ("<<row<<","<<col<<")\n";
                            }
                        } else { // Upgrade
                            if (gameMap.upgradeTower(row, col, oro)) {
                                ++upgradeCount;
                            } else {
                                std::cout << "Cannot upgrade tower at ("<<row<<","<<col<<")\n";
                            }
                        }
                    }
                }
            }
        }

        // Lógica de fases
        float elapsed = phaseClock.getElapsedTime().asSeconds();
        if (phase == Phase::Construction && elapsed >= BUILD_SECONDS) {
            phase = Phase::Wave;
            ++generaciones;

            // Posición inicial y destino
            sf::Vector2i entrada(0, 0);                         // [MAP_HEIGHT/2][0] en términos (col, fila)
            sf::Vector2i castillo(MAP_WIDTH - 1, MAP_HEIGHT-1); // suponiendo que el castillo está ahí

            // Calcular camino con A*
            std::vector<sf::Vector2i> camino = gameMap.findPathAStar(entrada, castillo);

            // Crear enemigo de prueba y asignar camino
            auto enemigo = std::make_unique<TestEnemy>();
            enemigo->setPath(camino);
            enemigos.push_back(std::move(enemigo));

        }

        //enemigos moviendose usando A*
        if (phase == Phase::Wave) {
            for (auto& e : enemigos) {
                e->moveEnemy();
            }
        }

        // Dibujado
        window.clear(sf::Color::Black);

        // Toolbar fondo
        sf::RectangleShape toolbar({ float(MAP_WIDTH*TILE_SIZE), float(TOOLBAR_HEIGHT) });
        toolbar.setFillColor({200,200,200});
        window.draw(toolbar);

        // Botones de torre (0..2)
        for (int i = 0; i < 3; ++i) {
            sf::RectangleShape btn({ float(TILE_SIZE-4), float(TOOLBAR_HEIGHT-4) });
            btn.setPosition({ float(i*TILE_SIZE+2), 2.f });
            bool active = (action==Action::Place && selectedTower==towerButtons[i]);
            btn.setFillColor(active
                ? sf::Color(150,150,150)
                : getColorForTile(towerButtons[i])
            );
            window.draw(btn);
        }

        // Botón Upgrade (índice 3)
        sf::RectangleShape upBtn({ float(TILE_SIZE-4), float(TOOLBAR_HEIGHT-4) });
        upBtn.setPosition({ float(3*TILE_SIZE+2), 2.f });
        bool upActive = (action==Action::Upgrade);
        upBtn.setFillColor(upActive ? sf::Color(150,150,150) : sf::Color::Yellow);
        window.draw(upBtn);

        // Etiqueta "U"
        sf::Text upLabel(font, "U", CHAR_SIZE);
        upLabel.setFillColor(sf::Color::Black);
        upLabel.setPosition({
            float(3*TILE_SIZE + 2 + (TILE_SIZE - CHAR_SIZE)/2),
            4.f
        });
        window.draw(upLabel);

        // Texto de fase
        std::string tbText = (phase == Phase::Construction)
            ? std::string("Construction Phase: ") + formatTime(BUILD_SECONDS - elapsed)
            : "Round Phase";
        sf::Text toolbarLabel(font, tbText, CHAR_SIZE);
        toolbarLabel.setFillColor(sf::Color::Black);
        toolbarLabel.setPosition({ float(TILE_SIZE + 200), 10.f });
        window.draw(toolbarLabel);

        // Mapa
        for (int r = 0; r < MAP_HEIGHT; ++r) {
            for (int c = 0; c < MAP_WIDTH; ++c) {
                sf::RectangleShape cell({ float(TILE_SIZE-2), float(TILE_SIZE-2) });
                cell.setPosition({
                    float(c*TILE_SIZE),
                    float(TOOLBAR_HEIGHT + r*TILE_SIZE)
                });
                cell.setFillColor(getColorForTile(gameMap.getTileType(r,c)));
                window.draw(cell);
            }
        }

        // Panel derecho
        sf::RectangleShape panel({ float(PANEL_WIDTH), float(MAP_HEIGHT*TILE_SIZE) });
        panel.setPosition({ float(MAP_WIDTH*TILE_SIZE), float(TOOLBAR_HEIGHT) });
        panel.setFillColor({230,230,230});
        window.draw(panel);

        // Estadísticas
        float x0 = MAP_WIDTH*TILE_SIZE + 10.f;
        float y0 = TOOLBAR_HEIGHT + 10.f;
        float dy = 22.f;
        sf::Text stat(font, "", CHAR_SIZE);
        stat.setFillColor(sf::Color::Black);

        if (phase == Phase::Construction) {
            stat.setString("Construction Phase");
            stat.setPosition({ x0, y0 }); window.draw(stat);
            stat.setString("Towers Placed: " + std::to_string(placedCount));
            stat.setPosition({ x0, y0 + dy }); window.draw(stat);
            stat.setString("Upgrades: " + std::to_string(upgradeCount));
            stat.setPosition({ x0, y0 + dy*2 }); window.draw(stat);
            stat.setString("Gold: " + std::to_string(oro));
            stat.setPosition({ x0, y0 + dy*3 }); window.draw(stat);
            stat.setString("Generation: " + std::to_string(generaciones));
            stat.setPosition({ x0, y0 + dy*4 }); window.draw(stat);
        } else {
            stat.setString("Round Phase");
            stat.setPosition({ x0, y0 }); window.draw(stat);
            stat.setString("Enemies Killed: " + std::to_string(enemigosMuertos));
            stat.setPosition({ x0, y0 + dy }); window.draw(stat);
            stat.setString("Gold: " + std::to_string(oro));
            stat.setPosition({ x0, y0 + dy*2 }); window.draw(stat);
            stat.setString("Fitness: " + std::to_string(fitnessActual));
            stat.setPosition({ x0, y0 + dy*3 }); window.draw(stat);
            stat.setString("Mutation%: " + std::to_string(int(probMutacion*100)) + "%");
            stat.setPosition({ x0, y0 + dy*4 }); window.draw(stat);
            stat.setString("Mutations: " + std::to_string(mutacionesAcumuladas));
            stat.setPosition({ x0, y0 + dy*5 }); window.draw(stat);
        }

        if (phase == Phase::Wave) {
            for (auto& e : enemigos) {
                e->draw(window);
            }
        }

        window.display();
    }

    return 0;
}
