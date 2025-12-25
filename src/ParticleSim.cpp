#include "ParticleSim.hpp"
#include <cmath>

#ifdef DEBUG
#include <stdio.h>
#endif

bool ParticleSim::collision_is_valid(CollisionEvent event) {
    if (NULL == event.particle_i) return false;
    if (event.version_i < event.particle_i->version) return false;
    if (this->t_now > event.time) return false;
    if (event.time > 1.0f) return false;  // event.time is absolute
    if (event.type == CollisionType::PARTICLE) {
        if (NULL == event.particle_j) return false;
        if (event.version_j < event.particle_j->version) return false;
    }
    if (event.type == CollisionType::EDGE) {
        sf::Vector2f v_delta = event.v_delta;
        float mag = std::sqrt((v_delta.x * v_delta.x) + (v_delta.y * v_delta.y));
        if (std::abs(mag) < EPS) return false;
    }
    return true;
}

p_sim_error_t ParticleSim::advance_time(float t_delta) {
    if (t_delta + this->t_now > 1.0f) this->t_now = 1.0f;
    else this->t_now += t_delta;
    return ERR_OK;
}

collision_status_t ParticleSim::time_of_particle_collision(
        float* t_coll, Particle* p_i, Particle* p_j) {
    if (NULL == t_coll) return COLLISION_ERR;
    sf::Vector2f dp = p_i->get_position() - p_j->get_position();
    sf::Vector2f dv = p_i->get_velocity() - p_j->get_velocity();
    float R = p_i->get_radius() + p_j->get_radius();
    float R_sq = R * R;
    float A = (dv.x * dv.x) + (dv.y * dv.y);        // dv * dv
    float B = ((dp.x * dv.x) + (dp.y * dv.y)) * 2.0f;        // dp * dv
    float C = ((dp.x * dp.x) + (dp.y * dp.y)) - R_sq; // position differential
    if (C <= 0.0f) return COLLISION_TRUE;              // not overlapping
    if (A < 1e-9) return COLLISION_FALSE;          // no relative motion (never will overlap)
    float D = (B * B) - (4 * A * C);                // discriminant
    if (D < 0.0f) return COLLISION_FALSE;            // no solution
    float sqrt_D = std::sqrt(D);                    // disc. sqrt
    float t1 = (-B - sqrt_D) / (2.0 * A);
    float t2 = (-B + sqrt_D) / (2.0 * A);
    float t_delta;
    if (t1 >= 0.0f) {
        t_delta = t1;
        if (t2 >= 0.0f && t2 < t1) {
            t_delta = t2;
        }
    } else if (t2 >= 0.0f) {
        t_delta = t2;
        if (t1 >= 0.0f && t1 < t2) {
            t_delta = t1;
        }
    } else {
        return COLLISION_FALSE;
    }
    if (t_delta + this->t_now > 1.0f) {
        return COLLISION_FALSE;
    }
    *t_coll = t_delta;
    return COLLISION_TRUE;
}

collision_status_t ParticleSim::check_for_collisions(uint32_t* n_collisions, Particle* p) {
    uint32_t backup_n_collisions;
    if (NULL == n_collisions) n_collisions = &backup_n_collisions;
    *n_collisions = 0;
    size_t queue_size = this->collision_queue.size();
    p_sim_error_t detect_res = this->field->detect_edge_collision(p->t_current, p, &this->collision_queue);
    if (ERR_OK != detect_res) return COLLISION_ERR;
    *n_collisions = this->collision_queue.size() - queue_size;
    float t_coll;
    for (Particle* o : this->particles) {
        if (p->id <= o->id) continue;
        collision_status_t res = time_of_particle_collision(&t_coll, p, o);
        switch (res) {
            case COLLISION_ERR:
                return COLLISION_ERR;
                break;
            case COLLISION_FALSE:
                continue;
                break;
            case COLLISION_TRUE:
                *n_collisions += 1;
                // t_coll is relative; convert to absolute time using the later of the two particles' times
                float t_base = p->t_current > o->t_current ? p->t_current : o->t_current;
                this->collision_queue.register_particle_collision(t_base + t_coll, p, o);
                break;
        }
    }
    *n_collisions += (uint32_t)(this->collision_queue.size() - queue_size);
    return *n_collisions > 0 ? COLLISION_TRUE : COLLISION_FALSE;
}

// TODO: handle race condition of overlapping particles
collision_status_t ParticleSim::collide(CollisionEvent event) {
    if (!collision_is_valid(event)) return COLLISION_FALSE;
    Particle* p_i = event.particle_i;
    Particle* p_j = event.particle_j;
    float collision_time = event.time;
    switch (event.type) {
        case CollisionType::EDGE: {
            this->advance_time(collision_time - t_now);
            p_i->advance(collision_time - p_i->t_current);
            p_i->add_velocity(event.v_delta);
            p_i->edge_collision_time = collision_time;
            // Correct position if particle is outside boundary
            sf::Vector2f field_center = sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y);
            sf::Vector2f to_particle = p_i->get_position() - field_center;
            float dist = std::sqrt(to_particle.x * to_particle.x + to_particle.y * to_particle.y);
            float max_dist = PARTICLE_FIELD_RADIUS - p_i->get_radius();
            if (dist > max_dist && dist > 0.0f) {
                sf::Vector2f corrected = field_center + to_particle * (max_dist / dist);
                p_i->set_position(corrected);
            }
            p_i->version++;
            return COLLISION_TRUE;
            break;
        }
        case CollisionType::PARTICLE:
            this->advance_time(collision_time - t_now);
            p_i->advance(collision_time - p_i->t_current);
            p_j->advance(collision_time - p_j->t_current);
            // resolve elastic collision
            sf::Vector2f dp = p_i->get_position() - p_j->get_position();
            float dp_length = std::sqrt(dp.x * dp.x + dp.y * dp.y);
            sf::Vector2f n = sf::Vector2f(dp.x / dp_length, dp.y / dp_length);
            sf::Vector2f dv = p_i->get_velocity() - p_j->get_velocity();
            float dv_dot_n = (dv.x * n.x + dv.y * n.y);
            if (dv_dot_n >= 0.0f) return COLLISION_FALSE;
            float m_i = p_i->get_mass();
            float m_j = p_j->get_mass();
            float inv_mass_sum = 1.0f / (m_i + m_j);
            float elastic_c = 1.0f;
            float impulse_magnitude = -(1.0f + elastic_c) * dv_dot_n * inv_mass_sum;
            sf::Vector2f impulse = n * impulse_magnitude;
#ifdef DEBUG
            // check for conservation of momentum
            sf::Vector2f v_i, v_j, mom_0, mom_1, mom_diff;
            v_i = p_i->get_velocity();
            v_j = p_j->get_velocity();
            mom_0 = v_i * p_i->get_mass() + v_j * p_j->get_mass();
#endif
            p_i->add_velocity(impulse * m_j);
            p_j->add_velocity(-impulse * m_i);
            // After impulse application
            const float total_r = p_i->get_radius() + p_j->get_radius();
            const float penetration = total_r - dp_length + 0.001f;
            if (penetration > 0.0f) {
                const float correction_factor = 0.8f;  // 80% fix per collision
                sf::Vector2f correction = n * (penetration * correction_factor * inv_mass_sum);
                p_i->set_position(p_i->get_position() + correction * m_j);
                p_j->set_position(p_j->get_position() - correction * m_i);
            }
#ifdef DEBUG
            v_i = p_i->get_velocity();
            v_j = p_j->get_velocity();
            mom_1 = v_i * p_i->get_mass() + v_j * p_j->get_mass();
            mom_diff = mom_1 - mom_0;
            float mom_error = std::hypot(mom_diff.x, mom_diff.y);
            if (mom_error > 0.001) {
                printf("Momentum changed by: %0.6f\n", mom_error);
            }
#endif
            p_i->version++;
            p_j->version++;
            return COLLISION_TRUE;
            break;
    }
    return COLLISION_ERR;
}

p_sim_error_t ParticleSim::process_collisions() {
    if (STATE_RUNNING != this->state) return ERR_INVALID_STATE;
    CollisionEvent* event;
    while (NULL != (event = this->collision_queue.pop())) {
        collision_status_t res = this->collide(*event);
        if (COLLISION_TRUE == res) {
            this->check_for_collisions(NULL, event->particle_i);
            if (CollisionType::PARTICLE == event->type) {
                this->check_for_collisions(NULL, event->particle_j);
            }
        }
        free(event);
    }
    return ERR_OK;
}

ParticleSim::ParticleSim(uint32_t n): n_particles(n) {
    this->state = STATE_INIT;
    this->origin = sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y);
    this->field = NULL;
}

ParticleSim::ParticleSim(uint32_t n, ParticleField* field): n_particles(n), field(field) {
    this->state = STATE_READY;
    this->origin = sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y);
    if (field == NULL) throw std::runtime_error("field is NULL!");
}

p_sim_error_t ParticleSim::assign_field(ParticleField* field) {
    if (field == NULL) return ERR_NULL_PTR;
    if (this->state != STATE_INIT || this->field != NULL) return ERR_INVALID_STATE;
    this->field = field;
    this->state = STATE_READY;
    return ERR_OK;
}

p_sim_error_t ParticleSim::begin() {
    if (this->field == NULL) return ERR_NULL_PTR;
    if (this->state != STATE_READY) return ERR_INVALID_STATE;
    if (ERR_OK != this->field->init(&this->particles, this->n_particles)) return ERR_FAIL;
    this->t_now = 0.0;
    this->state = STATE_RUNNING;
    return ERR_OK;
}

p_sim_error_t ParticleSim::update() {
    if (this->state != STATE_RUNNING) return ERR_INVALID_STATE;
    float v_max = 0.0f;
    sf::Vector2f momentum;
    this->t_now = 0.0;
    for (Particle* p : this->particles) {
        p->version = 0;
        p->t_current = 0.0f;
    }
    uint32_t n_collisions;
    for (Particle* p : this->particles) this->check_for_collisions(&n_collisions, p);
    p_sim_error_t res = this->process_collisions();
    if (ERR_OK != res) return res;
    for (Particle* p : this->particles) p->advance(1.0f - p->t_current);
    this->advance_time(1.0 - this->t_now); // not strictly necessary, but "correct"
#ifdef DEBUG
    float v_sum = 0.0f;
    for (Particle* p : this->particles) {
        sf::Vector2f v = p->get_velocity();
        float v_mag = std::hypot(v.x, v.y);
        v_max = v_mag > v_max ? v_mag : v_max;
        v_sum += v_mag;
        momentum += v * p->get_mass() / (float)PARTICLE_QUANTITY;
    }
    printf("v_max: %0.3f\n", v_max);
    printf("v_avg: %0.3f\n", v_sum / (float)PARTICLE_QUANTITY);
    printf("momentum magnitude: %0.3f\n", std::hypot(momentum.x, momentum.y));
#endif
    return ERR_OK;
}

p_sim_error_t ParticleSim::render(sf::RenderWindow* window) {
    for (Particle* p: this->particles) {
        p->render(window);
    }
    if (ERR_OK != this->field->render(window)) {
        return ERR_FAIL;
    }
    return ERR_OK;
}
