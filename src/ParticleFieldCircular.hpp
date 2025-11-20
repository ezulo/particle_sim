#ifndef __PARTICLEFIELDCIRCULAR_HPP__
#define __PARTICLEFIELDCIRCULAR_HPP__

#include <SFML/Graphics.hpp>
#include "Collision.hpp"
#include "Particle.hpp"
#include "ParticleField.hpp"
#include "config.h"
#include "p_sim_error.h"

typedef int32_t edge_collision_res_t;
const int32_t EDGE_COLLISION_OK   =  0;
const int32_t EDGE_COLLISION_NONE = -1;
const int32_t EDGE_COLLISION_ERR  = -2;

class ParticleFieldCircular : public ParticleField {
    private:
        sf::Color outline_color;
        sf::Vector2f position;
        std::vector<Particle*> virtual_particles;
        float radius;
        //sf::Vector2f reflect_velocity(Particle* p);
        p_sim_error_t time_of_collision(edge_collision_res_t* res, float* t_coll, Particle* p0);
        void reset_state();
        Particle* virtual_particle(Particle* p);
    public:
        ParticleFieldCircular(sf::Vector2f position, float radius, sf::Color);
        p_sim_error_t init(std::vector<Particle> p_list, uint32_t n_particles) override;
        p_sim_error_t detect_edge_collision(Particle* p, CollisionQueue* pq) override;
        p_sim_error_t render(sf::RenderWindow* window) override;
};

#endif
