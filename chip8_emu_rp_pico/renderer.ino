#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "renderer.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void initRenderer(renderer_t *renderer) {
    renderer->display = (uint8_t *)malloc(sizeof(uint8_t) * (renderer->cols) * (renderer->rows));
    memset(renderer->display, 0, (renderer->rows) * (renderer->cols));
}

uint8_t setPixel(renderer_t *renderer, uint8_t x, uint8_t y) {
    if (x >= renderer->cols) {
        x -= renderer->cols;
    } else if (x < 0) {
        x += renderer->cols;
    }

    if (y >= renderer->rows) {
        y -= renderer->rows;
    } else if (y < 0) {
        y += renderer->rows;
    }

    int pixelLocation = x + (y * renderer->cols);
    renderer->display[pixelLocation] ^= 1;

    return !renderer->display[pixelLocation];
}

void clear(renderer_t *renderer) {
    memset(renderer->display, 0, (renderer->cols) * (renderer->rows));
}

void render(renderer_t *renderer, Adafruit_SSD1306 display) {
    display.clearDisplay();
    for (size_t i = 0; i < (renderer->cols) * (renderer->rows); i++) {
        if (!renderer->display[i]) {
            continue;
        }
        uint16_t x = (i % renderer->cols) * (renderer->scale);
        uint16_t y = (i / renderer->cols) * (renderer->scale);

        display.fillRect(x, y, renderer->scale, renderer->scale, SSD1306_INVERSE);
        display.display();
    }
}
