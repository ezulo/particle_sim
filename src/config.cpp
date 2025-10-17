#include "config.h"
#include <cmath>

float rand_float(float min, float max) {
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}
