#ifndef __PARTICLEFIELDCIRCULAR_HPP__
#define __PARTICLEFIELDCIRCULAR_HPP__

#include <SFML/Graphics.hpp>
#include "CollisionQueue.hpp"
#include "Particle.hpp"
#include "ParticleField.hpp"
#include "config.h"
#include "p_sim_error.h"

typedef int32_t edge_collision_res_t;

/**
 * @brief A particle field bounded by a circular boundary.
 */
class ParticleFieldCircular : public ParticleField {
    private:
        sf::Color outline_color;
        sf::Vector2f position;
        std::vector<Particle*> virtual_particles;
        float radius;

        /**
         * @brief Calculates v_delta on collision of a particle with the field edge
         * @param p particle reference
         * @param t_coll time of collision (needed for proper calculation)
         */
        sf::Vector2f edge_collision_v_delta(Particle p, float t_coll);

        /**
         * @brief Calculates the time (with respect to simulation timestep) of a particle's
         * collision with the particle field boundary.
         *
         * @ param t_max maximum t_coll value
         * @ param t_coll where to store collision t_delta (if collision, 0.0 <= *t_coll < 1.0)
         * @ param p pointer to the particle in question
         * @ return COLLISION_TRUE if collided, COLLISION_FALSE if not, COLLISON_ERR for error
         */
        collision_status_t time_of_edge_collision(float t_max, float* t_coll, Particle* p);

    public:

        /**
         * @brief Particle Field (Circular) constructor
         *
         * @param position origin of circular field
         * @param radius radius of circular field
         * @param color color of field boundary
         * @return ParticleFieldCircular instance
         */
        ParticleFieldCircular(sf::Vector2f position, float radius, sf::Color);

        /** Abstract function overrides **/
        p_sim_error_t init(std::vector<Particle*>* p_list, uint32_t n_particles) override;
        p_sim_error_t detect_edge_collision(float t_now, Particle* p, CollisionQueue* pq) override;
        p_sim_error_t render(sf::RenderWindow* window) override;
        p_sim_error_t flush_state() override;

};

#endif
