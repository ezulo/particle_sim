#ifndef __PARTICLESIM_HPP__
#define __PARTICLESIM_HPP__

#include <stdint.h>
#include <SFML/Graphics.hpp>

#include "CollisionQueue.hpp"
#include "Particle.hpp"
#include "ParticleField.hpp"
#include "ParticleTracer.hpp"
#include "config.h"
#include "p_sim_error.h"

/**
 * @brief Particle simulation logic class. Handles collisions and timestep increment.
 */
class ParticleSim {
    private:
        uint32_t n_particles;
        ParticleField* field;
        std::vector<Particle*> particles;
        std::vector<ParticleTracer> tracers;
        CollisionQueue collision_queue;
        sf::Vector2f origin;
        sim_state_t state;
        float t_now;

        /**
         * @brief Checks validity of collision events against simulation state.
         * @param event collision event
         */
        bool collision_is_valid(CollisionEvent event);

        /**
         * @brief Advances time (t_now) by specified delta
         * @param t_delta the amount of time to advance (t_now + t_delta <= 1.0)
         * @return ERR_OK if successful
         */
        p_sim_error_t advance_time(float t_delta);

        /**
        * @brief helper function to find time of collision (if any) between 2 particles)
        * @param t_coll address of where to write collision time
        * @param p_i the first particle
        * @param p_j the second particle
        * @return COLLISION_TRUE if collided (collision time at *t_coll), COLLISION_FALSE if not
        */
        collision_status_t time_of_particle_collision(float* t_coll, Particle* p_i, Particle* p_j);

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
         * @param event the colision event
         * @return COLLISION_TRUE if valid. COLLISION_FALSE if not.
         */
        collision_status_t collide(CollisionEvent event);

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

        /**
         * @brief creates a tracer of a particle
         *
         * @return ERR_OK if successful
         */
        p_sim_error_t make_tracer(Particle* p);

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

