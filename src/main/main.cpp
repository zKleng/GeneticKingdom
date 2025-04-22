#include <SFML/Graphics.hpp>
int main()
{
    // 1) VideoMode ahora recibe un sf::Vector2u
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),    // Vector2u implícito, bitsPerPixel = 32 por defecto
        "SFML 3 Test"
    );

    sf::CircleShape shape(200.f);
    shape.setFillColor(sf::Color::Green);

    // 2) Bucle principal
    while (window.isOpen())
    {
        // 3) pollEvent() devuelve std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            // 4) Para comprobar cierres de ventana, usamos is<T>
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();       // clear() sin parámetros (o con clear(color))
        window.draw(shape);   // draw(expr) solo recibe la forma
        window.display();
    }

    return 0;
}
