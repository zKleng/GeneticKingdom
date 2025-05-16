#include "GeneticAlgorithm.h"
#include <algorithm>
#include <random>

void GeneticAlgorithm::select_and_reproduce(std::vector<std::unique_ptr<Enemy>>& population, double mutation_rate) {
    std::sort(population.begin(), population.end(),
              [](const std::unique_ptr<Enemy>& e1, const std::unique_ptr<Enemy>& e2) {
                  return e1->fitness() > e2->fitness();
              });

    std::vector<std::unique_ptr<Enemy>> new_population;

    for (size_t i = 0; i < population.size() / 2; i++) {
        auto& parent1 = population[i];
        auto& parent2 = population[i + 1];

        float new_health = (parent1->getHealth() + parent2->getHealth()) / 2;
        float new_speed = (parent1->getSpeed() + parent2->getSpeed()) / 2;
        float new_ra = (parent1->getResistanceArrow() + parent2->getResistanceArrow()) / 2;
        float new_rm = (parent1->getResistanceMagic() + parent2->getResistanceMagic()) / 2;
        float new_rt = (parent1->getResistanceArtillery() + parent2->getResistanceArtillery()) / 2;

        // Copiar padre y modificar genes
        std::unique_ptr<Enemy> child = parent1->clone();
        child->setGenes(new_health, new_speed, new_ra, new_rm, new_rt);

        // Mutación
        if (rand() % 100 < mutation_rate * 100) {
            child->setHealth(child->getHealth() + (rand() % 10 - 5));
            child->setSpeed(child->getSpeed() + (rand() % 5 - 2));
        }

        new_population.push_back(std::move(child));
    }

    population = std::move(new_population);
}