#include "ParticleFieldCircular.hpp"
#include <cmath>    // for std::pow() and std::sqrt()
#include <cstdlib>  // for rand() and RAND_MAX

#define T_NO_COLLIDE 1.0

/**
 * Static helper: generates a random float between min and max
 *
 * May be moved to a util class if we use it anywhere else
 *
 * @param min minimum float value
 * @param max maximum float value
 * @return a float value where (min <= value <= max)
 */
static float rand_float(float min, float max) {
    float rand_value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + rand_value * (max - min);
};

p_sim_error_t ParticleFieldCircular::time_of_edge_collision(float* t_coll, Particle* p) {
    if (NULL == t_coll) return ERR_INVALID_STATE;
    sf::Vector2f p_vel = p->get_velocity();
    sf::Vector2f p_pos = p->get_position();
    float        p_rad = p->get_radius();
    float effective_R = this->radius - p_rad;
    sf::Vector2f Q(p_pos.x - this->position.x, p_pos.y - this->position.y);
    sf::Vector2f D(p_vel.x, p_vel.y);
    float a = std::pow(D.x, 2) + std::pow(D.y, 2);
    float b = 2 * (Q.x * D.x + Q.y * D.y);
    float c = std::pow(Q.x, 2) + std::pow(Q.y, 2) - std::pow(effective_R, 2);
    float discriminant = std::pow(b, 2) - (4.0 * a * c);
    if (discriminant < 0) {
        *t_coll = T_NO_COLLIDE;
        return ERR_OK;
    }
    float sqrt_disc = std::sqrt(discriminant);
    float t1 = (b * -1.0 - sqrt_disc) / (2 * a);
    float t2 = (b * -1.0 + sqrt_disc) / (2 * a);
    if (t1 >= 0.0f) {
        if (t2 < 0.0f) {
            *t_coll = t1;
            return ERR_OK;
        }
        if (t1 < t2) {
            *t_coll = t1;
            return ERR_OK;
        }
    } else if (t2 >= 0.0f) {
        if (t1 < 0.0f) {
            *t_coll = t2;
            return ERR_OK;
        }
        if (t2 < t1) {
            *t_coll = t2;
            return ERR_OK;
        }
    }
    printf("There was an error getting t_coll\n");
    printf("p_pos: [%0.3f %0.3f]\n", p_pos.x, p_pos.y);
    printf("p_vel: [%0.3f %0.3f]\n", p_vel.x, p_vel.y);
    printf("t1: %0.3f t2: %0.3f\n", t1, t2);
    *t_coll = T_NO_COLLIDE;
    return ERR_COLLISION_CHECK_FAIL;
}

Particle* ParticleFieldCircular::create_virtual_particle(Particle* p) {
    // Create another particle with reflected velocity
    // Create virtual particle, reflected on field boundary
    float effective_R = this->radius - p->get_radius();
    sf::Vector2f P = p->get_position();
    sf::Vector2f N = (P - this->position) / effective_R;
    Particle* virt = new Particle(P + (N * p->get_radius()), p->get_radius(), sf::Color::Transparent, -1);
    // Assign velocity to virtual particle
    sf::Vector2f V_p = p->get_velocity();
    float V_dot = V_p.x * N.x + V_p.y * N.y;
    sf::Vector2f V_o = V_p - (N * (2.0f * V_dot));
    virt->set_velocity(V_o);
    this->virtual_particles.push_back(virt);
    return virt;
}

ParticleFieldCircular::ParticleFieldCircular(sf::Vector2f position, float radius, sf::Color color) {
    this->position = position;
    this->radius = radius;
    this->outline_color = color;
}

p_sim_error_t ParticleFieldCircular::init(std::vector<Particle> p_list, uint32_t n_particles) {
    try {
        for (uint32_t i = 0; i < n_particles; i++) {
            float angle = (float)rand_float(0, 2 * M_PI);
            float length = (float)rand_float(0, PARTICLE_FIELD_RADIUS - PARTICLE_RADIUS);
            sf::Vector2 position = sf::Vector2f(length * std::cos(angle), length * std::sin(angle));
            Particle p(position + this->position, PARTICLE_RADIUS, PARTICLE_COLOR, i);
            float rand_x = (float)rand_float(V0_MAX * -1.0f, V0_MAX);
            float rand_y = (float)rand_float(V0_MAX * -1.0f, V0_MAX);
            p.set_velocity(sf::Vector2f(rand_x, rand_y));
            p_list.push_back(p);
        }
        return ERR_OK;
    } catch (...) {
        return ERR_FAIL;
    }
}

p_sim_error_t ParticleFieldCircular::detect_edge_collision(Particle* p, CollisionQueue* pq) {
    float t_coll;
    p_sim_error_t err = time_of_edge_collision(&t_coll, p);
    if (ERR_OK != err) return err;
    if (T_NO_COLLIDE == t_coll) return ERR_OK;
    Particle* virt = this->create_virtual_particle(p);
    if (NULL == virt) return ERR_NO_MEMORY;
    pq->register_collision(t_coll, p, this->create_virtual_particle(p));
    return ERR_OK;
}

p_sim_error_t ParticleFieldCircular::render(sf::RenderWindow* window) {
    if (NULL == window) return ERR_NULL_PTR;
    try {
        sf::CircleShape p_field(this->radius);
        sf::Vector2f center_position = this->position - sf::Vector2f(this->radius, this->radius);
        p_field.setPosition(center_position);
        p_field.setOutlineColor(this->outline_color);
        p_field.setOutlineThickness(PARTICLE_FIELD_OUTLINE_THICKNESS);
        p_field.setFillColor(sf::Color::Transparent);
        window->draw(p_field);
    } catch (...) {
        return ERR_FAIL;
    }
    return ERR_OK;
}

p_sim_error_t ParticleFieldCircular::flush_state() {
    try {
        for (Particle* virtual_particle : this->virtual_particles) {
            delete virtual_particle;
        }
        this->virtual_particles.clear();
    } catch (...) {
        return ERR_FAIL;
    }
    return ERR_OK;
}

