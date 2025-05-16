#include "../../include/game/GeneticAlgorithm.h"
#include <algorithm>
#include <random>

void GeneticAlgorithm::select_and_reproduce(std::vector<std::unique_ptr<Enemy>>& population, double mutation_rate) {
    std::sort(population.begin(), population.end(),
              [](const std::unique_ptr<Enemy>& e1, const std::unique_ptr<Enemy>& e2) {
                  return e1->fitness() > e2->fitness();
              });

    std::vector<std::unique_ptr<Enemy>> new_population;

    size_t eliteCount = 1;  // Mantener al mejor individuo
    for (size_t i = 0; i < eliteCount && i < population.size(); ++i) {
        new_population.push_back(population[i]->clone());
    }

    for (size_t i = 0; i + 1 < population.size(); i += 2) {
        auto& parent1 = population[i];
        auto& parent2 = population[i + 1];

        float new_health = (parent1->getHealth() + parent2->getHealth()) / 2.f;
        float new_speed  = (parent1->getSpeed() + parent2->getSpeed()) / 2.f;
        float new_ra     = (parent1->getResistanceArrow() + parent2->getResistanceArrow()) / 2.f;
        float new_rm     = (parent1->getResistanceMagic() + parent2->getResistanceMagic()) / 2.f;
        float new_rt     = (parent1->getResistanceArtillery() + parent2->getResistanceArtillery()) / 2.f;

        // Mutación
        if (rand() % 100 < mutation_rate * 100) {
            new_health += (rand() % 16 + 5);   // +5 a +20
            new_speed  += ((rand() % 6) / 10.f); // +0.0 a +0.5
        }        

        // Crear hijo1
        auto child1 = parent1->clone();
        child1->setGenes(new_health, new_speed, new_ra, new_rm, new_rt);
        new_population.push_back(std::move(child1));

        // Crear hijo2 con genes invertidos
        auto child2 = parent2->clone();
        child2->setGenes(new_health, new_speed, new_ra, new_rm, new_rt);
        new_population.push_back(std::move(child2));
    }

    // Si sobran enemigos, completar hasta el tamaño original
    while (new_population.size() < population.size()) {
        new_population.push_back(population[0]->clone());
    }

    population = std::move(new_population);
}

