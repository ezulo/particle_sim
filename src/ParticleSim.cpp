#include "ParticleSim.hpp"
#include <cmath>

#ifdef DEBUG
#include <stdio.h>
#endif

collision_event_valid_t ParticleSim::collision_is_valid(CollisionEvent event) {
    if (NULL == event.particle_i) return INVALID_NULL_PTR_I;
    if (event.version_i < event.particle_i->version) {
        return INVALID_VER_I;
    }
    if (event.type == CollisionType::PARTICLE) {
        if (NULL == event.particle_j) return INVALID_NULL_PTR_J;
        if (event.version_j < event.particle_j->version) return INVALID_VER_J;
    }
    if (this->t_now > event.time) return INVALID_TIME_PASSED;
    if (this->t_now + event.time > 1.0) {
        return INVALID_TIME_OVERFLOW;
    }
    return VALID;
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
    this->field->detect_edge_collision(this->t_now, p, &this->collision_queue);
    // TODO: test edge collision first, then particle-on-particle
    collision_status_t res;
    float t_coll;
    for (Particle* o : this->particles) {
        if (p->id <= o->id) continue;
        res = time_of_particle_collision(&t_coll, p, o);
        if (COLLISION_ERR == res) return COLLISION_ERR;
        if (COLLISION_TRUE == res) {
            *n_collisions += 1;
            this->collision_queue.register_particle_collision(t_coll, p, o);
        }
    }
    *n_collisions += (uint32_t)(this->collision_queue.size() - queue_size);
    return *n_collisions > 0 ? COLLISION_TRUE : COLLISION_FALSE;
}

// TODO: handle race condition of overlapping particles
collision_status_t ParticleSim::collide(CollisionEvent event) {
    collision_event_valid_t valid_res = collision_is_valid(event);
    if (VALID != valid_res) {
        return COLLISION_FALSE;
    }
    Particle* p_i = event.particle_i;
    Particle* p_j = event.particle_j;
    float collision_time = event.time;
    float t_delta = collision_time - t_now;
    switch (event.type) {
        case CollisionType::EDGE:
            this->advance_time(t_delta);
            p_i->advance(t_delta);
            p_i->add_velocity(event.v_delta);
            p_i->version++;
            //printf("Edge collision (%d)\n", p_i->id);
            break;
        case CollisionType::PARTICLE:
            this->advance_time(t_delta);
            p_i->advance(t_delta);
            p_j->advance(t_delta);
            // resolve elastic collision
            sf::Vector2f dp = p_i->get_position() - p_j->get_position();
            float dp_length = std::sqrt(dp.x * dp.x + dp.y * dp.y);
            sf::Vector2f dp_normal = sf::Vector2f(dp.x / dp_length, dp.y / dp_length);
            sf::Vector2f dv = p_i->get_velocity() - p_j->get_velocity();
            float dv_dot = (dv.x * dp_normal.x + dv.y * dp_normal.y);
            if (dv_dot > 0) return COLLISION_FALSE;
            float impulse = 2.0 * dv_dot / 2.0; // assuming equal mass
            p_i->add_velocity(-1.0f * dp_normal * impulse);
            p_i->version++;
            p_j->add_velocity(1.0f * dp_normal * impulse);
            p_j->version++;
            //printf("Particle collision (%d -- %d)\n", p_i->id, p_j->id);
            break;
    }
    return COLLISION_TRUE;
}

p_sim_error_t ParticleSim::process_collisions() {
    if (STATE_RUNNING != this->state) return ERR_INVALID_STATE;
    CollisionEvent event;
    while (!this->collision_queue.empty()) {
        event = collision_queue.pop();
        collision_status_t res = this->collide(event);
        if (COLLISION_TRUE == collide(event)) {
            this->check_for_collisions(NULL, event.particle_i);
            if (CollisionType::PARTICLE == event.type) {
                this->check_for_collisions(NULL, event.particle_j);
            }
        }
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
    this->t_now = 0.0;
    for (Particle* p : this->particles) p->version = 0;
    uint32_t n_collisions;
    for (Particle* p : this->particles) this->check_for_collisions(&n_collisions, p);
    p_sim_error_t res = this->process_collisions();
    if (ERR_OK != res) return res;
    for (Particle* p : this->particles) p->advance(1.0 - this->t_now);
    this->advance_time(1.0 - this->t_now); // not strictly necessary, but "correct"
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
