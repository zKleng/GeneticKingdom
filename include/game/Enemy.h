#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>


//clase base de los enemigos que se usara para el polimorfismo de cada tipo especifico
class Enemy{
public:
    Enemy();//constructor
    virtual ~Enemy() = default;// Destructor virtual para polimorfismo

    // Métodos para la clase enemy
    virtual void moveEnemy();
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual float getStats() const;
    virtual void receiveDamage(float amount, std::string tipo); // tipo: flecha, magia, artillería
    //virtual void deleteEnemy(); funcion que elemina de la memoria cada enemigo que muere

    void setPath(const std::vector<sf::Vector2i>& camino);

protected:
    // Atributos que tendran las subclases de enemigos
    float health;
    float speed;
    float resistanceArrow;
    float resistanceMagic;
    float resistanceArtillery;

    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;

    //camino calculado con A*
    std::vector<sf::Vector2i> path;  // camino precalculado: lista de posiciones en la matriz
    int pathIndex = 0;               // en qué punto del camino va
};
