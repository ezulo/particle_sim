#ifndef __ERROR_H__
#define __ERROR_H__

typedef int32_t p_sim_error_t;

#define ERR_OK                      0x000
#define ERR_FAIL                    0x100
#define ERR_NULL_PTR                0x101
#define ERR_INVALID_STATE           0x102
#define ERR_NOT_IMPLEMENTED         0xFFF
#define ERR_NO_MEMORY               0x103
#define ERR_COLLISION_CHECK_FAIL    0x201

#endif
