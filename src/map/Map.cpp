#include "../../include/map/Map.h"

#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_map>
#include <cmath>

Map::Map() {
    grid.resize(MAP_HEIGHT, std::vector<MapTile>(MAP_WIDTH));

    // Entrada
    grid[MAP_HEIGHT/2][0] = MapTile(TileType::Entry);

    // Castillo (destino)
    grid[MAP_HEIGHT/2][MAP_WIDTH-1] = MapTile(TileType::Castle);
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


//Implementacion del algoritmo A* que usan los enemigos para encontrar el camino entre la salida y el castillo
struct Nodo {
    sf::Vector2i pos;
    float g;  // costo desde el inicio
    float f;  // g + heurística
    bool operator>(const Nodo& o) const { return f > o.f; }
};

static float heuristic(sf::Vector2i a, sf::Vector2i b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan
}

std::vector<sf::Vector2i> Map::findPathAStar(sf::Vector2i inicio, sf::Vector2i destino) const {
    std::priority_queue<Nodo, std::vector<Nodo>, std::greater<Nodo>> abiertos;
    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::unordered_map<int, float> costSoFar;

    auto hash = [](sf::Vector2i v) { return v.y * MAP_WIDTH + v.x; };

    abiertos.push({inicio, 0.f, heuristic(inicio, destino)});
    costSoFar[hash(inicio)] = 0.f;

    std::vector<sf::Vector2i> direcciones = {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    while (!abiertos.empty()) {
        Nodo actual = abiertos.top();
        abiertos.pop();

        if (actual.pos == destino) break;

        for (auto d : direcciones) {
            sf::Vector2i next = actual.pos + d;

            if (next.x < 0 || next.x >= MAP_WIDTH || next.y < 0 || next.y >= MAP_HEIGHT)
                continue;

            if (!grid[next.y][next.x].walkable && grid[next.y][next.x].type != TileType::Castle)
                continue;

            float newCost = costSoFar[hash(actual.pos)] + 1.f;
            if (!costSoFar.count(hash(next)) || newCost < costSoFar[hash(next)]) {
                costSoFar[hash(next)] = newCost;
                float priority = newCost + heuristic(next, destino);
                abiertos.push({next, newCost, priority});
                cameFrom[hash(next)] = actual.pos;
            }
        }
    }

    // Reconstrucción del camino
    std::vector<sf::Vector2i> path;
    sf::Vector2i current = destino;
    while (current != inicio) {
        path.push_back(current);
        if (cameFrom.find(hash(current)) == cameFrom.end()) {
            path.clear(); break; // no hay camino
        }
        current = cameFrom[hash(current)];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
