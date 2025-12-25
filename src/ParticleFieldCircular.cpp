#include "ParticleFieldCircular.hpp"
#include <cmath>    // for std::pow() and std::sqrt()
#include <cstdlib>  // for rand() and RAND_MAX

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

sf::Vector2f ParticleFieldCircular::edge_collision_v_delta(Particle p_1, float t_coll) {
    p_1.advance(t_coll);
    sf::Vector2f V = p_1.get_velocity();
    sf::Vector2f P = p_1.get_position() - this->position;
    float P_length = std::sqrt(P.x * P.x + P.y * P.y);
    sf::Vector2f N = P / P_length;
    float dot = V.x * N.x + V.y * N.y;
    return -N * (2.0f * dot);
}

collision_status_t ParticleFieldCircular::time_of_edge_collision(float t_max, float* t_coll, Particle* p) {
    if (!p || !t_coll) return COLLISION_ERR;
    *t_coll = 0.0f; // default collision value
    sf::Vector2f vel = p->get_velocity();
    sf::Vector2f pos = p->get_position();
    float        r   = p->get_radius();
    float        R   = this->radius - r;

    // Arena too small / Particle too big
    if (R <= 0.0f) return COLLISION_TRUE;

    sf::Vector2f dpos = pos - this->position;
    float a = vel.x * vel.x + vel.y * vel.y;
    float b = 2.0f * (dpos.x * vel.x + dpos.y * vel.y);
    float c = dpos.x * dpos.x + dpos.y * dpos.y - R * R;
    float discriminant = (b * b) - (4.0 * a * c);

    // Not moving, practically
    if (a < EPS) return COLLISION_FALSE;

    // Currently touching or outside
    if (c >= N_EPS) return COLLISION_TRUE;

    // No solution - no collision.
    if (discriminant < -1e-6f) return COLLISION_FALSE;

    // resolve t
    float t = -1.0f;
    float sqrt_disc = std::sqrt(discriminant);
    float t1 = (-b - sqrt_disc) / (2.0f * a);
    float t2 = (-b + sqrt_disc) / (2.0f * a);
    if (t1 >= 0.0f) t = t1;
    else if (t2 >= 0.0f) t = t2;

    // t is either invalid, or in a different timestep
    if (t < 0.0f || t > t_max + EPS) return COLLISION_FALSE;

    // collision looks good
    *t_coll = t;
    return COLLISION_TRUE;
}

ParticleFieldCircular::ParticleFieldCircular(sf::Vector2f position, float radius, sf::Color color) {
    this->position = position;
    this->radius = radius;
    this->outline_color = color;
}

p_sim_error_t ParticleFieldCircular::init(std::vector<Particle*>* p_list, uint32_t n_particles) {
    if (0 == n_particles) return ERR_INVALID_STATE;
    try {
        for (uint32_t i = 0; i < n_particles; i++) {
            float angle = (float)rand_float(0, 2 * M_PI);
            float length = (float)rand_float(0, PARTICLE_FIELD_RADIUS - PARTICLE_RADIUS_MAX - EPS);
            float p_radius = (float)rand_float(PARTICLE_RADIUS_MIN, PARTICLE_RADIUS_MAX);
            sf::Vector2 position = sf::Vector2f(length * std::cos(angle), length * std::sin(angle));
            Particle* p = new Particle(position + this->position, p_radius, PARTICLE_COLOR, i);
            float rand_x = (float)rand_float(V0_MAX * -1.0f, V0_MAX);
            float rand_y = (float)rand_float(V0_MAX * -1.0f, V0_MAX);
            p->set_velocity(sf::Vector2f(rand_x, rand_y));
            p_list->push_back(p);
        }
        return ERR_OK;
    } catch (...) {
        return ERR_FAIL;
    }
}

p_sim_error_t ParticleFieldCircular::detect_edge_collision(float t_now, Particle* p, CollisionQueue* pq) {
    float t_delta;
    collision_status_t res = time_of_edge_collision(1.0f - t_now, &t_delta, p);
    switch (res) {
        case COLLISION_ERR:
            return ERR_COLLISION_CHECK_FAIL;
            break;
        case COLLISION_FALSE:
            return ERR_OK;
            break;
        case COLLISION_TRUE:
            float t_final = t_now + t_delta;
            // final time is too soon from last collision
            if (t_final <= p->edge_collision_time + EPS) return ERR_OK;
            // final time is out of bounds this timestep
            if (t_final > 1.0f + EPS) return ERR_OK;
//#ifdef DEBUG
//            printf("Registering edge collision ( %d ) @ t %0.3f\n", p->id, t_final);
//#endif
            sf::Vector2f v_delta = this->edge_collision_v_delta(*p, t_delta);
            pq->register_edge_collision(t_final, p, v_delta);
            return ERR_OK;
            break;
    }
    // If we reach this, what are we even doing
    return ERR_INVALID_STATE;
}

p_sim_error_t ParticleFieldCircular::render(sf::RenderWindow* window) {
    if (NULL == window) return ERR_NULL_PTR;
    try {
        sf::CircleShape p_field(this->radius, 1000);
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

