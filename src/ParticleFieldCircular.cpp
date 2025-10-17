#include "ParticleFieldCircular.hpp"
#include <cmath>

float ParticleFieldCircular::time_of_collision(Particle* p0) {
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
        return COLLISION_NONE;
    }
    float sqrt_disc = std::sqrt(discriminant);
    float t1 = (b * -1.0 - sqrt_disc) / (2 * a);
    float t2 = (b * -1.0 + sqrt_disc) / (2 * a);
    if (t1 >= 0.0f) {
        if (t2 < 0.0f) return t1;
        if (t1 < t2) return t1;
    } else if (t2 >= 0.0f) {
        if (t1 < 0.0f) return t2;
        if (t2 < t1) return t2;
    }
    printf("There was an error getting t_coll\n");
    printf("P: [%0.3f %0.3f]\n", P.x, P.y);
    printf("V: [%0.3f %0.3f]\n", V.x, V.y);
    printf("t1: %0.3f t2: %0.3f\n", t1, t2);
    return COLLISION_ERR;
}
sf::Vector2f ParticleFieldCircular::reflect_velocity(Particle* p) {
    float effective_R = this->radius - p->get_radius();
    sf::Vector2f V_0 = p->get_velocity();
    sf::Vector2f P = p->get_position();
    sf::Vector2f N = (P - this->position) / effective_R;
    float dot = V_0.x * N.x + V_0.y * N.y;
    sf::Vector2f V_1 = V_0 - (N * (2.0f * dot ));
    return V_1;
}

ParticleFieldCircular::ParticleFieldCircular(
    sf::Vector2f position,
    float radius,
    sf::Color color)
{
    this->position = position;
    this->radius = radius;
    this->outline_color = color;
}
void ParticleFieldCircular::render(sf::RenderWindow* window) {
    sf::CircleShape p_field(this->radius);
    sf::Vector2f center_position = this->position - sf::Vector2f(this->radius, this->radius);
    p_field.setPosition(center_position);
    p_field.setOutlineColor(this->outline_color);
    p_field.setOutlineThickness(PARTICLE_FIELD_OUTLINE_THICKNESS);
    p_field.setFillColor(sf::Color::Transparent);
    window->draw(p_field);
}
void ParticleFieldCircular::edge_collision(Particle* p) {
    if (!p->enabled) return;
    float t_coll = time_of_collision(p);
    if (COLLISION_ERR == t_coll) {
        printf("Error in detection collision.\n");
        p->disable();
    }
    else if (t_coll > 1.0f) {
        p->update(1.0f);
    }
    else if (t_coll >= 0.0f && t_coll <= 1.0f) {
        p->update(t_coll);
        sf::Vector2f V_new = reflect_velocity(p);
        p->set_velocity(V_new);
        p->update(1.0f - t_coll);
    }
    else {
        printf("Unexpected t_coll value: %0.3f\n", t_coll);
        p->disable();
    }
}
