#include "../../include/map/Map.h"

Map::Map() {
    grid.resize(MAP_HEIGHT, std::vector<MapTile>(MAP_WIDTH));

    // Entrada
    grid[5][0] = MapTile(TileType::Entry);

    // --- Camino según tu imagen: ---

    // Fila superior (fila 0)
    for (int col = 1; col < 14; ++col)
        grid[1][col] = MapTile(TileType::Path);

    // Columna 13 descendente (de fila 0 a 5)
    for (int row = 1; row <= 5; ++row)
        grid[row][13] = MapTile(TileType::Path);

    // Columna 13 descendente (de fila 0 a 5)
    for (int row = 1; row <= 5; ++row)
        grid[row][1] = MapTile(TileType::Path);

    // Fila media (fila 5) de col 0 a 19
    for (int col = 0; col < 20; ++col)
        grid[5][col] = MapTile(TileType::Path);

    // Columna 13 descendente (de fila 0 a 5)
    for (int row = 6; row <= 10; ++row)
        grid[row][1] = MapTile(TileType::Path);
    // Conexiones adicionales
    for (int col = 13; col < 16; ++col) {
        grid[1][col] = MapTile(TileType::Path);
        grid[9][col] = MapTile(TileType::Path);
    }
    for (int row = 6; row <= 9; ++row) {
        grid[row][13] = MapTile(TileType::Path);
        grid[row][16] = MapTile(TileType::Path);
    }
    for (int row = 1; row <= 4; ++row)
        grid[row][16] = MapTile(TileType::Path);

    // Fila inferior (fila 10)
    for (int col = 1; col < 14; ++col)
        grid[10][col] = MapTile(TileType::Path);

    // Castillo (destino)
    grid[5][19] = MapTile(TileType::Castle);
}

bool Map::placeTower(int row, int col, TileType t, int &oro) {
    if (row<0||row>=MAP_HEIGHT||col<0||col>=MAP_WIDTH) return false;
    if (grid[row][col].type != TileType::Empty)          return false;

    int cost = tileCost(t);
    if (oro < cost || cost==0) return false;

    grid[row][col].type     = t;
    grid[row][col].walkable = false;
    oro -= cost;
    return true;
}

bool Map::upgradeTower(int row, int col, int &oro) {
    if (row<0||row>=MAP_HEIGHT||col<0||col>=MAP_WIDTH) return false;

    TileType current = grid[row][col].type;
    TileType next    = nextLevel(current);
    if (next == current) return false;

    int extraCost = tileCost(next) - tileCost(current);
    if (oro < extraCost) return false;

    grid[row][col].type     = next;
    grid[row][col].walkable = false;
    oro -= extraCost;
    return true;
}

TileType Map::getTileType(int row, int col) const {
    return grid[row][col].type;
}

bool Map::isWalkable(int row, int col) const {
    return grid[row][col].walkable;
}

void Map::printMap() const {
    for (int r = 0; r < MAP_HEIGHT; ++r) {
        for (int c = 0; c < MAP_WIDTH; ++c) {
            char ch = '.';
            switch (grid[r][c].type) {
                case TileType::Empty:  ch = '.'; break;
                case TileType::Path:   ch = '#'; break;
                case TileType::Entry:  ch = 'E'; break;
                case TileType::Castle: ch = 'C'; break;
                case TileType::Tower1: ch = '1'; break;
                case TileType::Tower2: ch = '2'; break;
                case TileType::Tower3: ch = '3'; break;
            }
            std::cout << ch;
        }
        std::cout << "\n";
    }
}
