#include "ParticleTracer.hpp"
#include <stdint.h>

#include "config.h"

#define MOD_R (PARTICLE_TRACER_MOD_R - PARTICLE_TRACER) / PARTICLE_TRACER
#define MOD_G (PARTICLE_TRACER_MOD_G - PARTICLE_TRACER) / PARTICLE_TRACER
#define MOD_B (PARTICLE_TRACER_MOD_B - PARTICLE_TRACER) / PARTICLE_TRACER

ParticleTracer::ParticleTracer(Particle *p, uint8_t timestep)
{
  this->position = p->get_position();
  this->color = p->get_color();
  this->radius = p->get_radius();
  this->timestep = timestep;
}

bool ParticleTracer::render(sf::RenderWindow *window)
{
  if (this->timestep == PARTICLE_TRACER)
  {
    this->timestep -= 1;
    return true;
  }
  if (this->timestep <= 0)
  {
    return false;
  }
  sf::Color fill_color = this->color;
  fill_color.r = fill_color.r + (MOD_R * (PARTICLE_TRACER - this->timestep));
  fill_color.g = fill_color.g + (MOD_G * (PARTICLE_TRACER - this->timestep));
  fill_color.b = fill_color.b + (MOD_B * (PARTICLE_TRACER - this->timestep));
  fill_color.a = (255 * this->timestep) / PARTICLE_TRACER;
#ifdef DEBUG
  printf("step %d: %d %d %d\n", this->timestep, fill_color.r, fill_color.g,
         fill_color.b);
#endif
  // fill_color.b);
  this->timestep -= 1;
  sf::CircleShape shape(this->radius, 100);
  sf::Vector2f center_position =
      this->position - sf::Vector2f(this->radius, this->radius);
  shape.setPosition(center_position);
  shape.setFillColor(fill_color);
  window->draw(shape);
  return true;
}
