#include "ParticleSim.hpp"

/**
* @brief helper functon to determine validity of event
* @param event CollisionEvent object
* @return true if valid, false if not
*/
bool collision_is_valid(CollisionEvent event) {
    if (NULL == event.particle_i || NULL == event.particle_j) return false;
    if (event.version_i < event.particle_i->version) return false;
    if (event.version_j < event.particle_j->version) return false;
    return true;
}

// TODO: implement
collision_status_t ParticleSim::check_for_collisions(uint32_t* n_collisions, Particle p) {
    // TODO: Check for field edge collisions
    // TODO: Check for particle collisions
    return COLLISION_ERR;
}

// TODO: implement
collision_status_t ParticleSim::collide(CollisionEvent* event) {
    if (NULL == event) return COLLISION_ERR;
    if (!collision_is_valid(*event)) return COLLISION_FALSE;
    // TODO: Update particle velocities, positions, and versions
    return COLLISION_ERR;
}

// TODO: implement
p_sim_error_t ParticleSim::process_collisions() {
    if (STATE_RUNNING != this->state) return ERR_INVALID_STATE;
    // TODO: Call `collide()` on all collision events in queue
    return ERR_NOT_IMPLEMENTED;
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
    if (ERR_OK != this->field->init(this->particles, n)) throw std::runtime_error("field init failed");
}

p_sim_error_t ParticleSim::assign_field(ParticleField* field) {
    if (field == NULL) return ERR_NULL_PTR;
    if (this->state != STATE_INIT || this->field != NULL) return ERR_INVALID_STATE;
    this->state = STATE_READY;
    return ERR_OK;
}

p_sim_error_t ParticleSim::begin() {
    if (this->field == NULL) return ERR_NULL_PTR;
    if (this->state != STATE_READY) return ERR_INVALID_STATE;
    if (ERR_OK != this->field->init(this->particles, this->n_particles)) return ERR_FAIL;
    this->state = STATE_RUNNING;
    return ERR_OK;
}

p_sim_error_t ParticleSim::update() {
    if (this->state != STATE_RUNNING) return ERR_INVALID_STATE;
    this->t_delta = 0.0;
    uint32_t n_collisions;
    for (Particle p : this->particles) {
        this->check_for_collisions(&n_collisions, p);
    }
    p_sim_error_t res = this->process_collisions();
    if (ERR_OK != res) return res;
    return ERR_OK;
}

p_sim_error_t ParticleSim::render(sf::RenderWindow* window) {
    for (Particle p: this->particles) {
        p.render(window);
    }
    this->field->render(window);
    return ERR_OK;
}
