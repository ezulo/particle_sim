#include "config.h"
#include "Particle.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>

void Particle::reset() {
    this->version = 0;
    this->edge_collision_time = -1.0f;
    this->t_current = 0.0f;
}

sf::Color Particle::speed_color() {
    sf::Color p_color, final_color;
    p_color = this->color;
    float p_speed = this->get_speed();
    if (p_speed > PARTICLE_SPEED_COLORS_MAX) p_speed = PARTICLE_SPEED_COLORS_MAX;
    int delta_r, delta_g, delta_b;
    float intensity = p_speed / PARTICLE_SPEED_COLORS_MAX;
    delta_r = static_cast<int>(PARTICLE_SPEED_COLOR_MOD_R * intensity);
    delta_g = static_cast<int>(PARTICLE_SPEED_COLOR_MOD_G * intensity);
    delta_b = static_cast<int>(PARTICLE_SPEED_COLOR_MOD_B * intensity);
    final_color = sf::Color(p_color.r + delta_r, p_color.g + delta_g, p_color.b + delta_b);
    //printf("particle %d (speed %0.3f) color: %d %d %d\n", this->id, this->get_speed(), final_color.r, final_color.g, final_color.b);
    return final_color;
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
    this->t_current = 0.0f;
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
float Particle::get_speed() {
    return sqrt((this->velocity.x * this->velocity.x) + (this->velocity.y * this->velocity.y));
}
float Particle::get_radius() {
    return this->radius;
}
float Particle::get_mass() {
    return this->mass;
}
sf::Vector2f Particle::get_momentum() {
    return this->mass * this->velocity;
}
sf::Color Particle::get_color() {
#ifdef PARTICLE_SPEED_COLORS
    if (PARTICLE_SPEED_COLORS == 1) {
        return this->speed_color();
    }
#endif
    return this->color;
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
    this->t_current += dt;
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
    particle_shape.setFillColor(this->get_color());
    window->draw(particle_shape);
    this->reset();
}

