#pragma once

// Ahora incluimos los tres tipos de torre
enum class TileType {
    Empty,
    Tower1,   // Roja
    Tower2,   // Azul
    Tower3,   // Verde
    Path,
    Entry,
    Castle
};

class MapTile {
public:
    TileType type;
    bool     walkable;

    MapTile(TileType tile = TileType::Empty)
      : type(tile),
        // Las tres torres bloquean el paso
        walkable(!(tile == TileType::Tower1 ||
                   tile == TileType::Tower2 ||
                   tile == TileType::Tower3))
    {}
};
