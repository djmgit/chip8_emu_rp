#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <stdint.h>
#include <Adafruit_SSD1306.h>

typedef struct renderer_t {
    uint8_t cols;
    uint8_t rows;
    uint8_t scale;
    uint8_t *display;
} renderer_t;

void initRenderer(renderer_t *renderer);
uint8_t setPixel(renderer_t *renderer, uint8_t, uint8_t);
void clear(renderer_t *renderer);
void render(renderer_t *renderer, Adafruit_SSD1306 display);

#endif