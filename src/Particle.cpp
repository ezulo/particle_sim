#include "config.h"
#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>

void Particle::reset() {
    this->version = 0;
    this->edge_collision_time = -1.0f;
}

Particle::Particle(sf::Vector2f position, float radius, sf::Color color, int id) {
    this->id = id;
    this->position = position;
    this->radius = radius;
    this->mass = radius * radius;
    this->color = color;
    this->velocity = sf::Vector2f(0.0f, 0.0f);
    this->enabled = true;
    this->version = 0;
    this->edge_collision_time = -1.0f;
}
sf::Vector2f Particle::get_position() {
    return this->position;
}
void Particle::set_position(sf::Vector2f position) {
    this->position = position;
}
sf::Vector2f Particle::get_velocity() {
    return this->velocity;
}
float Particle::get_radius() {
    return this->radius;
}
float Particle::get_mass() {
    return this->mass;
}
void Particle::set_velocity(sf::Vector2f v) {
    this->velocity = v;
}
void Particle::add_velocity(sf::Vector2f velocity_delta) {
    this->velocity += velocity_delta;
}
void Particle::advance(float dt) {
    if (PARTICLE_DISABLE_STOP && !this->enabled) return;
    this->position += dt * this->velocity;
}
void Particle::disable() {
    this->enabled = false;
}
void Particle::render(sf::RenderWindow* window) {
    if (PARTICLE_DISABLE_DISAPPEAR && !this->enabled) return;
    sf::CircleShape particle_shape(this->radius, 100);
    sf::Vector2f center_position = this->position - sf::Vector2f(
            this->radius, this->radius);
    particle_shape.setPosition(center_position);
    particle_shape.setFillColor(this->color);
    window->draw(particle_shape);
    this->reset();
}

