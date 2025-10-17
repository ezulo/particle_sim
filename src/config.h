#ifndef __PROJECTCONFIG_H__
#define __PROJECTCONFIG_H__

#define COLLISION_NONE -1.0
#define COLLISION_ERR  -2.0

#define PARTICLE_DISABLE_DISAPPEAR  0
#define PARTICLE_DISABLE_STOP       1

/* Resolution Options */
#define FRAMERATE       30.0f 
#define WINDOW_SIZE_X   1000
#define WINDOW_SIZE_Y   1000

/* Initial velocity max/min */
#define V0_MAX 5.0f

/* Sim options */
#define PARTICLE_RADIUS                     5.0f
#define PARTICLE_COLOR                      sf::Color::Red
#define PARTICLE_QUANTITY                   2000
#define PARTICLE_TRACER                     7
#define PARTICLE_FIELD_CENTER_X             500.0f
#define PARTICLE_FIELD_CENTER_Y             500.0f
#define PARTICLE_FIELD_OUTLINE_THICKNESS    3.0f
#define PARTICLE_FIELD_RADIUS               400.0f

/* Util */
float rand_float(float min, float max);

#endif

