#pragma once

enum class TileType {
    Empty,
    Tower,
    Path,
    Entry,
    Castle
};

class MapTile {
public:
    TileType type;
    bool walkable;

    MapTile(TileType t = TileType::Empty)
        : type(t), walkable(t != TileType::Tower) {}
};
