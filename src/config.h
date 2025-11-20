#ifndef __PROJECTCONFIG_H__
#define __PROJECTCONFIG_H__

#define PARTICLE_DISABLE_DISAPPEAR  0
#define PARTICLE_DISABLE_STOP       1

/* Resolution Options */
#define FRAMERATE       120.0f 
#define WINDOW_SIZE_X   1000
#define WINDOW_SIZE_Y   1000

/* Initial speed max/min */
#define V0_MAX 3.125f

/* Sim options */
#define PARTICLE_RADIUS                     5.0f
#define PARTICLE_COLOR                      sf::Color::Red
#define PARTICLE_QUANTITY                   300
#define PARTICLE_TRACER                     7
#define PARTICLE_FIELD_CENTER_X             500.0f
#define PARTICLE_FIELD_CENTER_Y             500.0f
#define PARTICLE_FIELD_OUTLINE_THICKNESS    3.0f
#define PARTICLE_FIELD_RADIUS               400.0f

/* Util */
float rand_float(float min, float max);

#endif

