#include <SFML/Graphics.hpp>

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <array>
#include <algorithm>
#include <iostream>   


//Constantes del juego para mas facil manejo

//cantidad de casillas de la matriz
const int MAP_WIDTH  = 50;
const int MAP_HEIGHT = 30;

// Constantes de tamaño
constexpr std::uint32_t TILE_SIZE      = MAP_WIDTH/2;
constexpr std::uint32_t TOOLBAR_HEIGHT = TILE_SIZE;
constexpr int           CHAR_SIZE      = 16;
constexpr std::uint32_t PANEL_WIDTH    = 200u;
constexpr float         BUILD_SECONDS  = 10.f;