#ifndef __PARTICLEFIELD_HPP__
#define __PARTICLEFIELD_HPP__

#include <SFML/Graphics.hpp>
#include "CollisionQueue.hpp"
#include "Particle.hpp"
#include "p_sim_error.h"

/**
 * @brief ABSTRACT class denoting a particle field, for use in particle simulation
 */
class ParticleField {
    public:
        /*
         * @brief Abstract function to generate N particles in the particle field.
         *
         * @param p_list Where particles will be stored.
         * @param n_particles Number of particles to create
         * @return ERR_OK if successful.
         */
        virtual p_sim_error_t init(std::vector<Particle> p_list, uint32_t n_particles) = 0;
        /*
         * @brief Abstract function to detect collisions with the field edge from t0 -> t1
         *
         * @param p Particle to check
         * @param pq Collision event queue to push to.
         * @return ERR_OK if check was successful (does not imply a collision)
         */
        virtual p_sim_error_t detect_edge_collision(Particle* p, CollisionQueue* pq) = 0;
        /*
         * @brief Abstract function to render the particle field (mainly its boundary)
         * @param window SFML window reference
         * @return ERR_OK if successful.
         */
        virtual p_sim_error_t render(sf::RenderWindow *window) = 0;
        /*
         * @brief Abstract function to flush a field state. Should be called post-render.
         *
         * For example, we use virtual particles to handle edge collisions in our sim. This will
         * remove all virtual particles created during registration of edge collisions.
         *
         * @return ERR_OK if successful.
         */
        virtual p_sim_error_t flush_state() = 0;
        /*
         * @brief Empty destructor (default)
         */
        virtual ~ParticleField() = 0;
};

#endif

