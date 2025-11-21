#include "CollisionQueue.hpp"

const sf::Vector2f v_delta_nil(0.0f, 0.0f);

p_sim_error_t CollisionQueue::register_edge_collision(
        float time, Particle* p, sf::Vector2f v_delta) {
    if (NULL == p) return ERR_NULL_PTR;
    this->data.push({ time, CollisionType::EDGE, v_delta, p, NULL, p->version, -1 });
    return ERR_OK;
}

p_sim_error_t CollisionQueue::register_particle_collision(float time, Particle* i, Particle* j) {
    if (NULL == i || NULL == j) return ERR_NULL_PTR;
    if (i->id > j->id) {
        this->data.push({ time, CollisionType::PARTICLE, v_delta_nil, i, j, i->version, j->version });
    } else {
        this->data.push({ time, CollisionType::PARTICLE, v_delta_nil, j, i, j->version, i->version });
    }
    return ERR_OK;
}

CollisionEvent CollisionQueue::pop() {
    CollisionEvent ret = this->data.top();
    this->data.pop();
    return ret;
}

bool CollisionQueue::empty() {
    return this->data.empty();
}

size_t CollisionQueue::size() {
    return this->data.size();
}

