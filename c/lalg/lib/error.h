//
// Created by zahfox on 9/8/19.
//

#ifndef LALG_DEMO_ERROR_H
#define LALG_DEMO_ERROR_H

#include "includes.h"

typedef enum LALG_ERR {
  LALG_ERR_UNDEFINED = 0,
  LALG_ERR_VEC_SIZE = 1,
} LALG_ERR;

void throw_err(LALG_ERR code);

#endif  // LALG_DEMO_ERROR_H
