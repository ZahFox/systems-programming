#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <time.h>

int msleep(long msec);

#define CLAMP(value, low, high) \
  (value < low) ? low : (value > high) ? high : value

#endif