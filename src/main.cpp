#include <stdint.h>
#include <cmath>
#include <vector>
#include <queue>
#include <stdio.h>

#include "ParticleSim.hpp"
#include "ParticleFieldCircular.hpp"
#include "config.h"
#include "p_sim_error.h"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), "SFML Application");
    window.setFramerateLimit(FRAMERATE);
    window.setPosition(sf::Vector2i(25, 55));
    ParticleSim sim = ParticleSim(PARTICLE_QUANTITY);
    ParticleFieldCircular field = ParticleFieldCircular(
        sf::Vector2f(PARTICLE_FIELD_CENTER_X, PARTICLE_FIELD_CENTER_Y),
        PARTICLE_FIELD_RADIUS,
        sf::Color::White
    );
    sim.assign_field((ParticleField*)&field);
    printf("Hello world\n");
    if (ERR_OK != sim.begin()) {
        printf("Failure beginning sim.\n");
        return 1;
    }
    printf("Sim has begun\n");
    uint32_t timestep = 0;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
        if (ERR_OK != sim.update()) {
            printf("Updating failure\n");
            return 1;
        }
        if (ERR_OK != sim.render(&window)) {
            printf("Rendering failure\n");
            return 1;
        }
        window.display();
        if (timestep == 200) break;
    }
}
