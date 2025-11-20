#ifndef __PARTICLEFIELD_HPP__
#define __PARTICLEFIELD_HPP__

#include <SFML/Graphics.hpp>
#include "Collision.hpp"
#include "Particle.hpp"
#include "p_sim_error.h"

// Abstract class
class ParticleField {
    public:
        virtual p_sim_error_t init(std::vector<Particle> p_list, uint32_t n_particles) = 0;
        virtual p_sim_error_t detect_edge_collision(Particle* p, CollisionQueue* pq) = 0;
        virtual p_sim_error_t render(sf::RenderWindow *window) = 0;
        virtual ~ParticleField() = 0;
};

#endif

