#ifndef __COLLISION_HPP__
#define __COLLISION_HPP__

#include "Particle.hpp"
#include <queue>

const float EPS = 1e-9;
const float INF = 1e30;

struct Collision {
    float time;
    Particle* particle_i;
    Particle* particle_j;
    bool operator>(const Collision& o) const {
        if (std::abs(time - o.time) > EPS) return time > o.time;
        return particle_i->id > o.particle_i->id;       // stable tie-break
    };
};

typedef std::priority_queue<Collision, std::vector<Collision>, std::greater<Collision>> CollisionQueue;

#endif

