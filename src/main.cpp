#include <stdint.h>
#include <cmath>
#include <vector>
#include <queue>

#include "Particle.hpp"
#include "ParticleFieldCircular.hpp"
#include "ParticleSim.hpp"
#include "config.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), "SFML Application");
    window.setFramerateLimit(FRAMERATE);
    window.setPosition(sf::Vector2i(25, 55));
    ParticleFieldCircular field = ParticleFieldCircular(
        sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y),
        PARTICLE_FIELD_RADIUS,
        sf::Color::White
    );
    ParticleSim sim = ParticleSim(PARTICLE_QUANTITY, &field);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
        sim.update();
        sim.render(&window);
        window.display();
    }
}
