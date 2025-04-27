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

    MapTile(TileType t = TileType::Empty)
      : type(t),
        // Las tres torres bloquean el paso
        walkable(!(t == TileType::Tower1 ||
                   t == TileType::Tower2 ||
                   t == TileType::Tower3))
    {}
};
