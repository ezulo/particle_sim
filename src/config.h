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
#define V0_MAX 20.0f

/* Sim options */
#define PARTICLE_RADIUS_MIN                 4.f
#define PARTICLE_RADIUS_MAX                 4.f
#define PARTICLE_COLOR                      sf::Color::White
#define PARTICLE_QUANTITY                   200
#define PARTICLE_ELASTIC_COEFF              1.00f

/* Tracers (to turn on, set PARTICLE_TRACER > 1) */
/* Optionally, can also modulate tracer colors (may conflict with speed coloring) */
#define PARTICLE_TRACER                     5
#define PARTICLE_TRACER_MOD_R               0
#define PARTICLE_TRACER_MOD_G               0
#define PARTICLE_TRACER_MOD_B               0

/* Field settings */
#define PARTICLE_FIELD_CENTER_X             500.0f
#define PARTICLE_FIELD_CENTER_Y             500.0f
#define PARTICLE_FIELD_OUTLINE_THICKNESS    1.0f
#define PARTICLE_FIELD_RADIUS               400.0f

/* Color particles based on speed (unsure how tracers will play with this) */
#define PARTICLE_SPEED_COLORS       1
#define PARTICLE_SPEED_COLORS_MAX   20.0f
#define PARTICLE_SPEED_COLOR_MOD_R  0
#define PARTICLE_SPEED_COLOR_MOD_G  -255
#define PARTICLE_SPEED_COLOR_MOD_B  -255


#define DEBUG 1

#endif

