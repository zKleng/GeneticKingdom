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

    //getter de los atributos
    float getHealth() const { return health; }
    float getSpeed() const { return speed; }
    float getResistanceArrow() const { return resistanceArrow; }
    float getResistanceMagic() const { return resistanceMagic; }
    float getResistanceArtillery() const { return resistanceArtillery; }

    //setters
    void setHealth(float h) { health = h; }
    void setSpeed(float s) { speed = s; }


    // Métodos para la clase enemy
    virtual void moveEnemy();
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void receiveDamage(float amount, std::string tipo); // tipo: flecha, magia, artillería
    //virtual void deleteEnemy(); funcion que elemina de la memoria cada enemigo que muere

    // Metodos para la implementa del A* en los enemigos
    void setPath(const std::vector<sf::Vector2i>& camino);
    virtual sf::Vector2f getPosition() const;

    // Metodos para la implementacion del algoritmo genetico
    virtual float getStats() const;
    virtual float fitness() const;
    virtual std::unique_ptr<Enemy> clone() const = 0;
    virtual void setGenes(float h, float s, float ra, float rm, float rt) = 0;


protected:
    // Atributos que tendran las subclases de enemigos
    float health;                 // Vida
    float speed;                  // Velocidad
    float resistanceArrow;        // Resistencia a las flechas
    float resistanceMagic;        // Resistencia a la magia
    float resistanceArtillery;    // Resistencia a la artillería

    // Creacion del visual del enemigo.
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;

    //camino calculado con A*
    std::vector<sf::Vector2i> path;  // camino precalculado: lista de posiciones en la matriz
    int pathIndex = 0;               // en qué punto del camino va
};
