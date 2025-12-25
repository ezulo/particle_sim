#ifndef __PROJECTCONFIG_H__
#define __PROJECTCONFIG_H__

#define PARTICLE_DISABLE_DISAPPEAR  0
#define PARTICLE_DISABLE_STOP       1

/* Resolution Options */
#define FRAMERATE       60.0f 
#define WINDOW_SIZE_X   1000
#define WINDOW_SIZE_Y   1000

/* Nice Consts for Floating Point ops */
#define EPS         1e-8f
#define N_EPS        -1e-6f

/* Initial speed max/min */
#define V0_MAX 7.0f

/* Sim options */
#define PARTICLE_RADIUS_MIN                 10.0f
#define PARTICLE_RADIUS_MAX                 10.0f
#define PARTICLE_COLOR                      sf::Color::Blue
#define PARTICLE_QUANTITY                   200
#define PARTICLE_TRACER                     7
#define PARTICLE_FIELD_CENTER_X             500.0f
#define PARTICLE_FIELD_CENTER_Y             500.0f
#define PARTICLE_FIELD_OUTLINE_THICKNESS    1.0f
#define PARTICLE_FIELD_RADIUS               400.0f

#define DEBUG 1

#endif

