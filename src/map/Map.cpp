#include "../../include/map/Map.h"

Map::Map() {
    grid.resize(MAP_HEIGHT, std::vector<MapTile>(MAP_WIDTH));

    // Entrada
    grid[5][0] = MapTile(TileType::Entry);

    // --- Camino según tu imagen: ---

    // Fila superior (fila 0)
    for (int col = 0; col < 14; ++col)
        grid[0][col] = MapTile(TileType::Path);

    // Columna 13 descendente (de fila 0 a 5)
    for (int row = 0; row <= 5; ++row)
        grid[row][13] = MapTile(TileType::Path);

    // Fila media (fila 5) de col 0 a 19
    for (int col = 0; col < 20; ++col)
        grid[5][col] = MapTile(TileType::Path);

    // Fila 1 de Columna 13 a 16 
    for (int col = 13; col < 16; ++col)
        grid[1][col] = MapTile(TileType::Path);

    // Fila 9 Columna 13 a 16
    for (int col = 13; col < 16; ++col)
        grid[9][col] = MapTile(TileType::Path);
    // Columna 13 descendente (de fila 6 a 9)
    for (int row = 6; row <= 9; ++row)
        grid[row][13] = MapTile(TileType::Path);

    for (int row = 6; row <= 9; ++row)
        grid[row][16] = MapTile(TileType::Path);

    for (int row = 1; row <= 4; ++row)
        grid[row][16] = MapTile(TileType::Path);

    // Fila inferior (fila 10)
    for (int col = 0; col < 14; ++col)
        grid[10][col] = MapTile(TileType::Path);

    // Castillo (destino)
    grid[5][19] = MapTile(TileType::Castle);
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
