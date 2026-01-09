#ifndef __PARTICLEFIELD_HPP__
#define __PARTICLEFIELD_HPP__

#include "CollisionEvent.hpp"
#include "Particle.hpp"
#include "p_sim_error.h"
#include <SFML/Graphics.hpp>

/**
 * @brief ABSTRACT class denoting a particle field, for use in particle
 * simulation
 */
class ParticleField
{
public:
  /*
   * @brief Abstract function to generate N particles in the particle field.
   *
   * @param p_list Where particles will be stored.
   * @param n_particles Number of particles to create
   * @return ERR_OK if successful.
   */
  virtual p_sim_error_t init(std::vector<Particle *> *p_list,
                             uint32_t n_particles) = 0;
  /*
   * @brief Abstract function to detect collisions with the field edge from t0
   * -> t1
   *
   * @param t_now current time
   * @param p Particle to check
   * @param cev Collision event vector to push to.
   * @param t_delta Pointer to store t_delta
   * @return ERR_OK if check was successful (does not imply a collision)
   */
  virtual p_sim_error_t
  detect_edge_collision(float t_now, Particle *p,
                        std::vector<CollisionEvent> *cev) = 0;
  /*
   * @brief Abstract function to render the particle field (mainly its boundary)
   * @param window SFML window reference
   * @return ERR_OK if successful.
   */
  virtual p_sim_error_t render(sf::RenderWindow *window) = 0;
  /*
   * @brief Abstract function to flush a field state. Should be called
   * post-render.
   *
   * For example, we use virtual particles to handle edge collisions in our sim.
   * This will remove all virtual particles created during registration of edge
   * collisions.
   *
   * @return ERR_OK if successful.
   */
  virtual p_sim_error_t flush_state() = 0;
  /*
   * @brief Empty destructor (default)
   */
  virtual ~ParticleField() = 0;
};

#endif
