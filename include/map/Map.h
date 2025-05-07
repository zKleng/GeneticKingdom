#pragma once

#include <vector>
#include <iostream>
#include "MapTile.h"

#include <SFML/Graphics.hpp>

static inline int tileCost(TileType t) {
    switch (t) {
        case TileType::Tower1: return 10;
        case TileType::Tower2: return 20;
        case TileType::Tower3: return 50;
        default:               return 0;
    }
}

// Devuelve el siguiente nivel de torre (o el mismo si no es upgradable)
static inline TileType nextLevel(TileType t) {
    switch (t) {
        case TileType::Tower1: return TileType::Tower2;
        case TileType::Tower2: return TileType::Tower3;
        default:               return t;
    }
}

const int MAP_WIDTH  = 50;
const int MAP_HEIGHT = 25;

class Map {
private:
    std::vector<std::vector<MapTile>> grid;

public:
    Map();

    // Coloca una torre del tipo dado, descontando 'oro'. Devuelve true si se colocó.
    bool placeTower(int row, int col, TileType t, int &oro);

    // Mejora la torre existente al siguiente nivel, descontando la diferencia de coste.
    bool upgradeTower(int row, int col, int &oro);

    TileType getTileType(int row, int col) const;
    bool     isWalkable(int row, int col) const;

    // Para debug por consola
    void printMap() const;

    // Camino calculado usando A*
    std::vector<sf::Vector2i> findPathAStar(sf::Vector2i inicio, sf::Vector2i destino) const;
};
