#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdint.h>

typedef int32_t p_sim_error_t;
#define ERR_OK 0x000
#define ERR_FAIL 0x100
#define ERR_NULL_PTR 0x101
#define ERR_INVALID_STATE 0x102
#define ERR_NOT_IMPLEMENTED 0xFFF
#define ERR_NO_MEMORY 0x103
#define ERR_NO_DATA 0x104
#define ERR_COLLISION_CHECK_FAIL 0x201
#define ERR_COLLISION_FAIL 0x202

typedef int32_t collision_status_t;
#define COLLISION_TRUE 1
#define COLLISION_FALSE 0
#define COLLISION_ERR -1

typedef uint8_t sim_state_t;
#define STATE_INIT 0
#define STATE_READY 1
#define STATE_RUNNING 2
#define STATE_FINISHED 3

#endif
