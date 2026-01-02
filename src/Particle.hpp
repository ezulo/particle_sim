#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <SFML/Graphics.hpp>
#include <stdint.h>

class Particle {
    private:
        sf::Vector2f position;
        sf::Vector2f velocity;
        float radius;
        float mass;
        sf::Color color;
        void reset();
        sf::Color speed_color();
    public:
        int id;
        int version;
        bool enabled;
        float edge_collision_time;
        float t_current;  // tracks particle's current time within the timestep
        Particle(sf::Vector2f position, float radius, sf::Color color, int id);
        sf::Vector2f get_position();
        void set_position(sf::Vector2f position);
        sf::Vector2f get_velocity();
        float get_speed();
        float get_radius();
        float get_mass();
        sf::Vector2f get_momentum();
        sf::Color get_color();
        void set_velocity(sf::Vector2f v);
        void add_velocity(sf::Vector2f v);
        void advance(float dt);
        void disable();
        void render(sf::RenderWindow* window);
};

#endif
