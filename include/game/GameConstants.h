#pragma once

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <array>
#include <algorithm>
#include <iostream>   


//Constantes del juego para mas facil manejo

//cantidad de casillas de la matriz
const int MAP_WIDTH  = 40;
const int MAP_HEIGHT = 25;

// Constantes de tamaño
constexpr std::uint32_t TILE_SIZE      = MAP_WIDTH/2;
constexpr std::uint32_t TOOLBAR_HEIGHT = TILE_SIZE+50;
constexpr int           CHAR_SIZE      = 10;
constexpr std::uint32_t PANEL_WIDTH    = 150u;
constexpr float         BUILD_SECONDS  = 30.f;

//Constantes de posicion del castillo y salida de enemigos
const int CASTLE_PLACEMENT_X=MAP_WIDTH-1;
const int CASTLE_PLACEMENT_Y=MAP_HEIGHT-1;//modificar la altura en la que se coloca el castillo
const int ENEMY_ENTRY_X=MAP_HEIGHT/2;//modifica la altura de salida de los enemigos
const int ENEMY_ENTRY_Y=0;