#include "../../include/map/Map.h"
#include "../../include/game/GameConstants.h"
#include "../../include/game/Ogro.h"
#include "../../include/game/ElfoOscuro.h"
#include "../../include/game/Harpia.h"
#include "../../include/game/Mercenario.h"
#include "../../include/game/Arquero.h"
#include "../../include/game/Mago.h"
#include "../../include/game/Artillero.h"
#include "../../include/game/GeneticAlgorithm.h"

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
        /*case TileType::Tower1:  return sf::Color::Red;
        case TileType::Tower2:  return sf::Color::Blue;
        case TileType::Tower3:  return sf::Color::Green;*/
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

    // Enemigos y torres activas
    std::vector<std::unique_ptr<Enemy>> enemigos; //vector (lista ordenada) que guarda los punteros dinamicamente de cada enemigo
    std::vector<std::unique_ptr<Tower>> torres; //vector (lista ordenada) que guarda los punteros dinamicamente de cada torre


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
    
    sf::Clock deltaClock;

    while (window.isOpen()) {

        //timer que usan las torres para atacar
        float deltaTime = deltaClock.restart().asSeconds();

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
                            
                                if (selectedTower == TileType::Tower1) {
                                    auto torre = std::make_unique<ArqueroTower>();
                                    torre->setPosition(sf::Vector2f(static_cast<float>(col * TILE_SIZE), static_cast<float>(TOOLBAR_HEIGHT + row * TILE_SIZE)));
                                    torre->setGridPosition(row, col);
                                    torres.push_back(std::move(torre));
                                } else if(selectedTower == TileType::Tower2){
                                    auto torre = std::make_unique<MagoTower>();
                                    torre->setPosition(sf::Vector2f(static_cast<float>(col * TILE_SIZE), static_cast<float>(TOOLBAR_HEIGHT + row * TILE_SIZE)));
                                    torre->setGridPosition(row, col);
                                    torres.push_back(std::move(torre));
                                } else if(selectedTower == TileType::Tower3){
                                    auto torre = std::make_unique<ArtilleroTower>();
                                    torre->setPosition(sf::Vector2f(static_cast<float>(col * TILE_SIZE), static_cast<float>(TOOLBAR_HEIGHT + row * TILE_SIZE)));
                                    torre->setGridPosition(row, col);
                                    torres.push_back(std::move(torre));
                                }
                            
                            } else {
                                std::cout << "Cannot place tower at ("<<row<<","<<col<<")\n";
                            }
                        } else { // Upgrade
                            bool upgraded = false;
                            for (auto& torre : torres) {
                                auto [tRow, tCol] = torre->getGridPosition();
                                if (tRow == row && tCol == col) {
                                    if (torre->upgradeTower(oro)) {
                                        ++upgradeCount;
                                        upgraded = true;
                                        std::cout << "Torre mejorada. Nivel actual: " << torre->getLevel() << "\n";
                                    } else {
                                        std::cout << "No se mejoro la torre, nivel maximo o falta de oro\n";
                                    }
                                    break;
                                }
                            }
                            if (!upgraded) {
                                std::cout << "No tower found at (" << row << "," << col << ")\n";
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
            sf::Vector2i entrada(ENEMY_ENTRY_Y, ENEMY_ENTRY_X); // en términos (col, fila) punto de salida de los enemigos
            sf::Vector2i castillo(CASTLE_PLACEMENT_X, CASTLE_PLACEMENT_Y); // posicion castillo

            // Calcular camino con A*
            std::vector<sf::Vector2i> camino = gameMap.findPathAStar(entrada, castillo);

            // Creacion de enemigos

            // Creacion ogros
            for (int i = 0; i < 1; ++i) {
                auto enemigo = std::make_unique<Ogro>();
                enemigo->setPath(camino);
                enemigos.push_back(std::move(enemigo));
            }

            // Creacion elfos
            for (int i = 0; i < 0; ++i) {
                auto enemigo = std::make_unique<ElfoOscuro>();
                enemigo->setPath(camino);
                enemigos.push_back(std::move(enemigo));
            }

            // Creacion harpias
            for (int i = 0; i < 0; ++i) {
                auto enemigo = std::make_unique<Harpia>();
                enemigo->setPath(camino);
                enemigos.push_back(std::move(enemigo));
            }

            // Creacion mercenarios
            for (int i = 0; i < 0; ++i) {
                auto enemigo = std::make_unique<Mercenario>();
                enemigo->setPath(camino);
                enemigos.push_back(std::move(enemigo));
            }

        }

        //enemigos moviendose usando A*
        if (phase == Phase::Wave) {
            for (auto& e : enemigos) {
                e->moveEnemy();
            }
            for (auto& torre : torres) {
                torre->attackEnemy(enemigos, deltaTime, oro, enemigosMuertos);
            }
        
            if (enemigos.empty()) {
                // Calcular fitness de la generación actual
                for (auto& enemy : enemigos) {
                    enemy->fitness();
                }
        
                // Aplicar algoritmo genético
                GeneticAlgorithm::select_and_reproduce(enemigos, probMutacion);
        
                // Mostrar resultados
                std::cout << "=== Generación " << generaciones << " finalizada ===\n";
                for (const auto& enemy : enemigos) {
                    std::cout << "Enemy: salud=" << enemy->getHealth()
                              << ", velocidad=" << enemy->getSpeed()
                              << ", fitness=" << enemy->fitness() << "\n";
                }
        
                // Preparar próxima oleada
                phase = Phase::Construction;
                phaseClock.restart();
                ++generaciones;
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


        // Dibuja el mapa (las celdas de fondo)
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

        // Dibuja las torres (que están encima del mapa)
        for (auto& torre : torres) {
            torre->draw(window);
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

            for (auto& torre : torres) {
                torre->attackEnemy(enemigos, deltaTime, oro, enemigosMuertos);
            }
        }

        window.display();
    }

    return 0;
}
