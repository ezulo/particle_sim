#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <SFML/Graphics.hpp>
#include <stdint.h>

class Particle {
    private:
        sf::Vector2f position;
        sf::Vector2f velocity;
        float radius;
        sf::Color color;
    public:
        bool enabled;
        Particle(sf::Vector2f position, float radius, sf::Color color);
        sf::Vector2f get_position();
        sf::Vector2f get_velocity();
        float get_radius();
        void set_velocity(sf::Vector2f v);
        void add_velocity(sf::Vector2f v);
        void update(float dt);
        void disable();
        void render(sf::RenderWindow* window);
};

#endif
