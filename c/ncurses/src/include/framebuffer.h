#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "common.h"

#define MAX_DEPTH 0xFF

typedef struct __framebuffer {
  const unsigned int width, height;
  uint16_t* const color_buffer;
  uint8_t* const depth_buffer;
} framebuffer;

framebuffer* new_fb(unsigned int width, unsigned int height);

void free_fb(framebuffer* buffer);

void fb_clear(framebuffer* buffer, unsigned int clear_color);

void fb_print(framebuffer* buffer, unsigned int offset_x,
              unsigned int offset_y);

uint16_t fb_get_color(framebuffer* buffer, unsigned int x, unsigned int y);
uint8_t fb_get_depth(framebuffer* buffer, unsigned int x, unsigned int y);

#endif