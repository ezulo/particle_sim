#ifndef __PARTICLESIM_HPP__
#define __PARTICLESIM_HPP__

#include <cmath>
#include <stdint.h>
#include <SFML/Graphics.hpp>

typedef int32_t collision_status_t;
#define COLLISION_TRUE   1
#define COLLISION_FALSE  0
#define COLLISION_ERR    -1

typedef uint8_t sim_state_t;
#define STATE_INIT          0
#define STATE_READY         1
#define STATE_RUNNING       2
#define STATE_FINISHED      3

#include "CollisionQueue.hpp"
#include "Particle.hpp"
#include "ParticleField.hpp"
#include "config.h"
#include "p_sim_error.h"

/**
 * @brief Particle simulation logic class. Handles collisions and timestep increment.
 */
class ParticleSim {
    private:
        uint32_t n_particles;
        ParticleField* field;
        std::vector<Particle> particles;
        CollisionQueue collision_queue;
        sf::Vector2f origin;
        sim_state_t state;
        float t_delta;
        /**
         * @brief Checks particle for collision against other particles, and field boundaries.
         *
         * If collisions occur, will be pushed to `collision_queue`.
         *
         * @param n_collisions location to store number of collisions
         * @param p location of particle data to check
         * @return COLLISION_TRUE if any collisions occurred (*n_collisions > 0)
         */
        collision_status_t check_for_collisions(uint32_t* n_collisions, Particle* p);

        /**
         * @brief Applies collision to particles.
         *
         * If the collision is valid, updates particle positions + velocities accordingly, and
         * increments paricle versions for this timestep.
         *
         * If collision is valid, all particles in the event should be checked again for collisions
         * against field boundaries and other particles, and any further collisions should be
         * appended to `collision_queue`.`
         *
         * @return COLLISION_TRUE if valid. COLLISION_FALSE if not.
         */
        collision_status_t collide(CollisionEvent* event);

        /**
         * @brief Process collisions in CollisionQueue for timestep t0 -> t1
         *
         * In implementation, will iterate collision events from collision_queue, apply the
         * collision events using `collide()` (and call `check_for_collisions()` if collisions are
         * valid), and repeat until `collision_queue` is empty.
         *
         * @return ERR_OK if successful
         */
        p_sim_error_t process_collisions();

    public:
        /**
         * @brief ParticleSim constructor (with NULL field)
         * @return Sim in the STATE_INIT state.
         */
        ParticleSim(uint32_t n);

        /**
         * @brief ParticleSim constructor with pre-instantiated field.
         * Will call `ParticleField::init()` and ready the simulation.
         * @return Sim in the STATE_READY state.
         */
        ParticleSim(uint32_t n, ParticleField* field);

        /**
         * @brief Assigns a particle field, if not already assigned.
         * @param field ParticleField pointer
         * @return ERR_OK if successful.
         */
        p_sim_error_t assign_field(ParticleField* field);

        /**
         * @brief Begins the particle sim.
         * @return ERR_OK if successful.
         */
        p_sim_error_t begin();

        /**
         * @brief Updates the particle sim to the next timestep. Collision checking will occur.
         * @return ERR_OK if successful.
         */
        p_sim_error_t update();

        /**
         * @brief Renders the simulation (particles + field boundary) onto SFML Window.
         * @param window SFML window
         * @return ERR_OK if successful
         */
        p_sim_error_t render(sf::RenderWindow* window);

};

#endif

