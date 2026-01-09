#ifndef __COLLISION_EVENT_HPP__
#define __COLLISION_EVENT_HPP__

#include "Particle.hpp"
#include <SFML/System/Vector2.hpp>

const float EPS = 1e-9;
const float INF = 1e30; // may need this for particle <-> particle calculations

using namespace std;

/**
 * @brief enum to delineate collision types
 * EDGE is a particle against a field edge
 * PARTICLE is a particle against another particle
 */
enum CollisionType
{
  EDGE,
  PARTICLE
};

/**
 * @brief struct to keep track of collisions between particles
 *
 * To be stored in CollisionQueue type. Items will be popped from CollisionQueue
 * in order of time, and the particle's version will increment as collisions are
 * calculated. If the versions of the referenced particles exceed the version
 * recorded at collision time, the collision is discarded.
 *
 * Note: should this be a class, at this point? Does it complicate
 * priority_queue use?
 *
 */
struct CollisionEvent
{
  float time;              // registered time of collision (0.0 <= time <= 1.0)
  enum CollisionType type; // type (EDGE / PARTICLE)
  sf::Vector2f v_delta;    // (EDGE only) applied velocity delta on collision
  Particle *particle_i;    // particle i pointer
  Particle *particle_j;    // (PARTICLE only) particle j pointer
  int version_i;           // version of particle i at collision time
  int version_j; // (PARTICLE only) version of particle j at collision time

  /** @brief '>' operator override, to order Collisions by time. */
  bool operator>(const CollisionEvent &o) const
  {
    if (std::abs(time - o.time) > EPS)
      return time > o.time;
    return particle_i->id > o.particle_i->id; // stable tie-break
  };

  /** @brief '<' operator override for priority_queue (min-heap by time). */
  bool operator<(const CollisionEvent &o) const
  {
    if (std::abs(time - o.time) > EPS)
      return time > o.time; // Inverted: larger time = lower priority
    return particle_i->id > o.particle_i->id;
  };
};

#endif
