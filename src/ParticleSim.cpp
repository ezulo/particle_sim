#include "ParticleSim.hpp"

ParticleSim::ParticleSim(uint32_t n, ParticleField* field): 
    n_particles(n), field(field)
{
    this->origin = sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y);
    for (uint32_t i = 0; i < n_particles; i++) {
        // generate position
        float angle = rand_float(0, 2 * M_PI);
        float length = rand_float(0, PARTICLE_FIELD_RADIUS - PARTICLE_RADIUS);
        sf::Vector2 position = sf::Vector2f(length * std::cos(angle), length * std::sin(angle));
        Particle p(position + this->origin, PARTICLE_RADIUS, PARTICLE_COLOR);
        float rand_x = rand_float(V0_MAX * -1.0f, V0_MAX);
        float rand_y = rand_float(V0_MAX * -1.0f, V0_MAX);
        p.set_velocity(sf::Vector2f(rand_x, rand_y));
        this->particles.push_back(p);
    }
}
void ParticleSim::update() {
    for (uint32_t i = 0; i < this->n_particles; i++) {
        Particle p = this->particles[i];
        this->field->edge_collision(&p);
        this->particles[i] = p;
    }
}
void ParticleSim::render(sf::RenderWindow* window) {
    for (Particle p: this->particles) {
        p.render(window);
    }
    this->field->render(window);
}
