#ifndef __COLLISION_QUEUE_HPP__
#define __COLLISION_QUEUE_HPP__

#include <queue>
#include "Particle.hpp"
#include "p_sim_error.h"

const float EPS = 1e-9;
const float INF = 1e30; //may need this for particle <-> particle calculations

using namespace std;

/**
 * @brief struct to keep track of collisions between particles
 *
 * To be stored in CollisionQueue type. Items will be popped from CollisionQueue in order of time,
 * and the particle's version will increment as collisions are calculated. If the versions of the
 * referenced particles exceed the version recorded at collision time, the collision is discarded.
 * 
 * Note: should this be a class, at this point? Does it complicate priority_queue use?
 *
 */
struct CollisionEvent {
    float time;             // registered time of collision (0.0 <= time <= 1.0)
    Particle* particle_i;   // particle i pointer
    Particle* particle_j;   // particle j pointer
    int version_i;     // version of particle i at collision time
    int version_j;     // version of particle j at collision time

    /** @brief '>' operator override, to order Collisions by time. */
    bool operator>(const CollisionEvent& o) const {
        if (std::abs(time - o.time) > EPS) return time > o.time;
        return particle_i->id > o.particle_i->id; // stable tie-break
    };
};

class CollisionQueue {
    private:
        priority_queue<CollisionEvent, vector<CollisionEvent>, greater<CollisionEvent>> data;
    public:
        /**
         * @brief default constructor
         */
        CollisionQueue() {};

        /**
        * @brief helper function to register collisions
        * @param time time of collision with respect to sim timestep (0.0 <= time <= 1.0)
        * @param i Particle i location
        * @param j Particle j location
        * @return ERR_OK on success
        */
        p_sim_error_t register_collision(float time, Particle* i, Particle* j);

        /**
         * @brief retrieves soonest registered CollisionEvent in the timestep.
         *
         * Retrieves and removes the soonest CollisionEvent from the queue.
         *
         * @param event location to store CollisionEvent data. Will be malloc'd if NULL. (must be
         * freed after use!)
         * @return ERR_OK on success
         */
        p_sim_error_t pop(CollisionEvent* event);
};

#endif
