#pragma once

#include <vector>
#include <iostream>
#include "MapTile.h"

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 11;

class Map {
private:
    std::vector<std::vector<MapTile>> grid;

public:
    Map();

    void placeTower(int row, int col);
    TileType getTileType(int row, int col) const;
    bool isWalkable(int row, int col) const;

    // Para depurar por consola (opcional)
    void printMap() const;
};
