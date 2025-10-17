#ifndef __PARTICLEFIELDCIRCULAR_HPP__
#define __PARTICLEFIELDCIRCULAR_HPP__

#include <SFML/Graphics.hpp>

#include "Particle.hpp"
#include "ParticleField.hpp"
#include "config.h"

class ParticleFieldCircular : public ParticleField {
    private:
        sf::Vector2f position;
        float radius;
        sf::Color outline_color;
        float time_of_collision(Particle* p0);
        sf::Vector2f reflect_velocity(Particle* p);
    public:
        ParticleFieldCircular(sf::Vector2f position, float radius, sf::Color);
        void render(sf::RenderWindow* window) override;
        void edge_collision(Particle* p) override;
};

#endif
