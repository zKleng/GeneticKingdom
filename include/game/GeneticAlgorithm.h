#pragma once
#include "Enemy.h"
#include <vector>
#include <memory>

class GeneticAlgorithm {
public:
    // Función para seleccionar y evolucionar a los enemigos
    static void select_and_reproduce(std::vector<std::unique_ptr<Enemy>>& population, double mutation_rate);
};