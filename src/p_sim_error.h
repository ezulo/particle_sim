#ifndef __ERROR_H__
#define __ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t p_sim_error_t;
const int ERR_OK = 0;
const int ERR_FAIL = -1;

const int ERR_NULL_PTR = 0x101;
const int ERR_INVALID_STATE = 0x102;

const int ERR_COLLISION_CHECK_FAIL = 0x201;

#ifdef __cplusplus
}
#endif

#endif
