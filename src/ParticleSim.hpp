#ifndef __PARTICLESIM_H__
#define __PARTICLESIM_H__

#include <cmath>
#include <stdint.h>
#include <SFML/Graphics.hpp>
#include "config.h"
#include "ParticleField.hpp"

class ParticleSim {
    private:
        uint32_t n_particles;
        ParticleField* field;
        std::vector<Particle> particles;
        sf::Vector2f origin;
    public:
        ParticleSim(uint32_t n, ParticleField* field);
        void update();
        void render(sf::RenderWindow* window);
};

#endif

