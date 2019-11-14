//
// Created by zahfox on 9/8/19.
//

#include "error.h"

void* throw_err(LALG_ERR code) {
  switch (code) {
    case LALG_ERR_UNDEFINED: {
      fputs("Error (LALG_ERR_UNDEFINED): Encountered an unexpected error.",
            stderr);
      break;
    }
    case LALG_ERR_VEC_SIZE: {
      fputs("Error (LALG_ERR_VEC_SIZE): Vector sizes are restricted to 1-4.",
            stderr);
      break;
    }
    default: {
      fputs("Error (LALG_ERR_UNDEFINED): Encountered an unexpected error.",
            stderr);
      break;
    }
  }
  exit(1);
}
