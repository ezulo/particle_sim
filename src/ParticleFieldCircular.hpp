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
         * @brief Calculates the time (with respect to simulation timestep) of a particle's
         * collision with the particle field boundary.
         *
         * @ param t_coll where to store collision t_delta (if collision, 0.0 <= *t_coll < 1.0)
         * @ param p pointer to the particle in question
         * @ return ERR_OK for success. t_coll == 1.0 if no collision occurred.
         */
        p_sim_error_t time_of_edge_collision(float* t_coll, Particle* p);

        /**
         * @brief Creates a "particle" reflected along collision tangent, to process collision
         * against the field edge. This way, our sim can treat collisions with the field edge as
         * any other collision with a particle. This particle is not to be rendered.
         *
         * @param p reference particle to reflect along field edge.
         * @return Pointer to new virtual particle, otherwise NULL if failed
         */
        Particle* create_virtual_particle(Particle* p);

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
        p_sim_error_t init(std::vector<Particle> p_list, uint32_t n_particles) override;
        p_sim_error_t detect_edge_collision(Particle* p, CollisionQueue* pq) override;
        p_sim_error_t render(sf::RenderWindow* window) override;
        p_sim_error_t flush_state() override;

};

#endif
