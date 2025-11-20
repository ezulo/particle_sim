#include "ParticleFieldCircular.hpp"
#include <cmath>

void ParticleFieldCircular::reset_state() {
    for (Particle* virtual_particle : this->virtual_particles) {
        delete virtual_particle;
    }
    this->virtual_particles.clear();
}

p_sim_error_t ParticleFieldCircular::time_of_collision(
        edge_collision_res_t* res, float* t_coll, Particle* p0) {
    if (NULL == res || NULL == t_coll) return ERR_INVALID_STATE;
    sf::Vector2f V = p0->get_velocity();
    sf::Vector2f P = p0->get_position();
    float effective_R = this->radius - p0->get_radius();
    sf::Vector2f Q(P.x - this->position.x, P.y - this->position.y);
    sf::Vector2f D(V.x, V.y);
    float a = std::pow(D.x, 2) + std::pow(D.y, 2);
    float b = 2 * (Q.x * D.x + Q.y * D.y);
    float c = std::pow(Q.x, 2) + std::pow(Q.y, 2) - std::pow(effective_R, 2);
    float discriminant = std::pow(b, 2) - (4.0 * a * c);
    if (discriminant < 0) {
        *res = EDGE_COLLISION_NONE;
        return ERR_OK;
    }
    float sqrt_disc = std::sqrt(discriminant);
    float t1 = (b * -1.0 - sqrt_disc) / (2 * a);
    float t2 = (b * -1.0 + sqrt_disc) / (2 * a);
    if (t1 >= 0.0f) {
        if (t2 < 0.0f) {
            *res = EDGE_COLLISION_OK;
            *t_coll = t1;
            return ERR_OK;
        }
        if (t1 < t2) {
            *res = EDGE_COLLISION_OK;
            *t_coll = t1;
            return ERR_OK;
        }
    } else if (t2 >= 0.0f) {
        if (t1 < 0.0f) {
            *res = EDGE_COLLISION_OK;
            *t_coll = t2;
            return ERR_OK;
        }
        if (t2 < t1) {
            *res = EDGE_COLLISION_OK;
            *t_coll = t2;
            return ERR_OK;
        }
    }
    printf("There was an error getting t_coll\n");
    printf("P: [%0.3f %0.3f]\n", P.x, P.y);
    printf("V: [%0.3f %0.3f]\n", V.x, V.y);
    printf("t1: %0.3f t2: %0.3f\n", t1, t2);
    *res = EDGE_COLLISION_ERR;
    return ERR_COLLISION_CHECK_FAIL;
}

//sf::Vector2f ParticleFieldCircular::reflect_velocity(Particle* p) {
//    float effective_R = this->radius - p->get_radius();
//    sf::Vector2f V_0 = p->get_velocity();
//    sf::Vector2f P = p->get_position();
//    sf::Vector2f N = (P - this->position) / effective_R;
//    float dot = V_0.x * N.x + V_0.y * N.y;
//    sf::Vector2f V_1 = V_0 - (N * (2.0f * dot ));
//    return V_1;
//}

Particle* ParticleFieldCircular::virtual_particle(Particle* p) {
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

p_sim_error_t ParticleFieldCircular::render(sf::RenderWindow* window) {
    try {
        sf::CircleShape p_field(this->radius);
        sf::Vector2f center_position = this->position - sf::Vector2f(this->radius, this->radius);
        p_field.setPosition(center_position);
        p_field.setOutlineColor(this->outline_color);
        p_field.setOutlineThickness(PARTICLE_FIELD_OUTLINE_THICKNESS);
        p_field.setFillColor(sf::Color::Transparent);
        window->draw(p_field);
        this->reset_state();
    } catch (...) {
        return ERR_FAIL;
    }
    return ERR_OK;
}

//float ParticleFieldCircular::edge_collision(Particle* p) {
//    if (!p->enabled) return;
//    float t_coll = time_of_collision(p);
//    if (COLLISION_ERR == t_coll) {
//        printf("Error in detection collision.\n");
//        p->disable();
//    }
//    else if (t_coll > 1.0f) {
//        return COLLISION_NONE;
//    }
//    else if (t_coll >= 0.0f && t_coll <= 1.0f) {
//        return t_coll;
//    }
//    else {
//        printf("Unexpected t_coll value: %0.3f\n", t_coll);
//        p->disable();
//    }
//}

p_sim_error_t ParticleFieldCircular::detect_edge_collision(Particle* p, CollisionQueue* pq) {
    int res;
    float t_coll;
    time_of_collision(&res, &t_coll, p);
    switch (res) {
        case EDGE_COLLISION_NONE:
            return ERR_OK;
            break;
        case EDGE_COLLISION_ERR:
            return ERR_COLLISION_CHECK_FAIL;
            break;
        case EDGE_COLLISION_OK:
            pq->push({t_coll, p, this->virtual_particle(p)});
            break;
        default:
            return ERR_INVALID_STATE;
            break;
    }
    return ERR_OK;
}
