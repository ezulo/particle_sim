#include "CollisionQueue.hpp"

p_sim_error_t CollisionQueue::register_collision(float time, Particle* i, Particle* j) {
    if (NULL == i || NULL == j) return ERR_INVALID_STATE;
    this->data.push({ time, i, j, i->version, j->version });
    return ERR_OK;
}

p_sim_error_t CollisionQueue::pop(CollisionEvent* event) {
    if (NULL == event) event = (CollisionEvent*)malloc(sizeof(CollisionEvent));
    if (NULL == event) return ERR_NO_MEMORY;
    try {
        *event = this->data.top();
        this->data.pop();
    } catch (...) {
        free(event);
        return ERR_INVALID_STATE;
    }
    return ERR_OK;
}

