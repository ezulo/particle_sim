#include "ParticleSim.hpp"

#define STATE_INIT          0
#define STATE_READY         1
#define STATE_RUNNING       2
#define STATE_FINISHED      3

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
    for (uint32_t i = 0; i < this->n_particles; i++) {
        Particle p = this->particles[i];
        this->field->detect_edge_collision(&p, &this->collision_queue);
        this->particles[i] = p;
    }
    return ERR_OK;
}

p_sim_error_t ParticleSim::render(sf::RenderWindow* window) {
    for (Particle p: this->particles) {
        p.render(window);
    }
    this->field->render(window);
    return ERR_OK;
}
