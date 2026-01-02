#ifndef __PARTICLETRACER_HPP__
#define __PARTICLETRACER_HPP__

#include "Particle.hpp"
#include <SFML/Graphics.hpp>

class ParticleTracer {
    private:
        sf::Vector2f position;
        sf::Color color;
        uint8_t timestep;
        float radius;
    public:
        ParticleTracer(Particle* p, uint8_t timestep);
        bool render(sf::RenderWindow* window);
};

#endif

