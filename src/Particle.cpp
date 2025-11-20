#include "config.h"
#include "Particle.hpp"
#include <SFML/Graphics.hpp>

Particle::Particle(sf::Vector2f position, float radius, sf::Color color, int id) {
    this->id = id;
    this->position = position;
    this->radius = radius;
    this->color = color;
    this->velocity = sf::Vector2f(0.0f, 0.0f);
    this->enabled = true;
    this->version = 0;
}
sf::Vector2f Particle::get_position() {
    return this->position;
}
sf::Vector2f Particle::get_velocity() {
    return this->velocity;
}
float Particle::get_radius() {
    return this->radius;
}
void Particle::set_velocity(sf::Vector2f v) {
    this->velocity = v;
}
void Particle::add_velocity(sf::Vector2f velocity_delta) {
    this->velocity += velocity_delta;
}
void Particle::update(float dt) {
    if (PARTICLE_DISABLE_STOP && !this->enabled) return;
    this->position += dt * this->velocity;
    this->version++;
}
void Particle::disable() {
    this->enabled = false;
}
void Particle::render(sf::RenderWindow* window) {
    if (PARTICLE_DISABLE_DISAPPEAR && !this->enabled) return;
    sf::CircleShape particle_shape(this->radius);
    sf::Vector2f center_position = this->position - sf::Vector2f(
            this->radius, this->radius);
    particle_shape.setPosition(center_position);
    particle_shape.setFillColor(this->color);
    window->draw(particle_shape);
    this->version = 0;
}

