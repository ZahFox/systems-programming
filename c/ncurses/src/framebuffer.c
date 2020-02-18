#include "framebuffer.h"

framebuffer* new_fb(unsigned int width, unsigned int height) {
  framebuffer* instance = malloc(sizeof(framebuffer));
  *(int*)&instance->width = width;
  *(int*)&instance->height = height;

  *(uint16_t**)&instance->color_buffer =
      malloc(sizeof(uint16_t) * width * height);

  *(uint8_t**)&instance->depth_buffer =
      malloc(sizeof(uint8_t) * width * height);

  return instance;
}

void free_fb(framebuffer* buffer) {
  free(buffer->depth_buffer);
  free(buffer->color_buffer);
  free(buffer);
}

void fb_clear(framebuffer* buffer, unsigned int clear_color) {
  memset(buffer->color_buffer, clear_color,
         sizeof(uint16_t) * buffer->width * buffer->height);

  memset(buffer->depth_buffer, clear_color,
         sizeof(uint8_t) * buffer->width * buffer->height);
}

void fb_print(framebuffer* buffer, unsigned int offset_x,
              unsigned int offset_y) {}

uint16_t fb_get_color(framebuffer* buffer, unsigned int x, unsigned int y) {
  unsigned int index = (y * buffer->width) + x;
  return buffer->color_buffer[index];
}

uint8_t fb_get_depth(framebuffer* buffer, unsigned int x, unsigned int y) {
  unsigned int index = (y * buffer->width) + x;
  return buffer->depth_buffer[index];
}