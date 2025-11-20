#ifndef __PARTICLESIM_HPP__
#define __PARTICLESIM_HPP__

#include <cmath>
#include <stdint.h>
#include <SFML/Graphics.hpp>
#include "Collision.hpp"
#include "Particle.hpp"
#include "ParticleField.hpp"
#include "config.h"
#include "p_sim_error.h"

class ParticleSim {
    private:
        uint32_t n_particles;
        ParticleField* field;
        std::vector<Particle> particles;
        CollisionQueue collision_queue;
        sf::Vector2f origin;
        int state;
    public:
        ParticleSim(uint32_t n);
        ParticleSim(uint32_t n, ParticleField* field);
        p_sim_error_t assign_field(ParticleField* field);
        p_sim_error_t begin();
        p_sim_error_t update();
        p_sim_error_t render(sf::RenderWindow* window);
};

#endif

