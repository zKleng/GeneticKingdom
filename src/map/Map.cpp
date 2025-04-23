#include "../../include/map/Map.h"

Map::Map() {
    grid.resize(MAP_HEIGHT, std::vector<MapTile>(MAP_WIDTH));

    // Entrada de enemigos
    grid[5][0] = MapTile(TileType::Entry);

    // Castillo al otro extremo
    grid[5][MAP_WIDTH - 1] = MapTile(TileType::Castle);
}

void Map::placeTower(int row, int col) {
    if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH)
        return;

    if (grid[row][col].type == TileType::Empty) {
        grid[row][col].type = TileType::Tower;
        grid[row][col].walkable = false;
    }
}

TileType Map::getTileType(int row, int col) const {
    return grid[row][col].type;
}

bool Map::isWalkable(int row, int col) const {
    return grid[row][col].walkable;
}

void Map::printMap() const {
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            char symbol = '.';
            switch (grid[row][col].type) {
                case TileType::Entry:   symbol = 'E'; break;
                case TileType::Castle:  symbol = 'C'; break;
                case TileType::Tower:   symbol = 'T'; break;
                default: break;
            }
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}
