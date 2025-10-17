#ifndef __PARTICLEFIELD_HPP__
#define __PARTICLEFIELD_HPP__

#include <SFML/Graphics.hpp>
#include "Particle.hpp"

// Abstract class
class ParticleField {
    public:
        virtual void edge_collision(Particle* p) = 0;
        virtual void render(sf::RenderWindow *window) = 0;
        virtual ~ParticleField() = 0;
};

#endif

