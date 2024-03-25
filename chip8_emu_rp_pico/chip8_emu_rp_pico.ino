#include "renderer.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define SCALE 2
#define TARGET_FPS 60

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(CHIP8_SCREEN_WIDTH * SCALE, CHIP8_SCREEN_HEIGHT * SCALE);

renderer_t renderer = {
    .cols = CHIP8_SCREEN_WIDTH,
    .rows = CHIP8_SCREEN_HEIGHT,
    .scale = SCALE,
    .display = NULL
};

void setup() {
    Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  display.clearDisplay();
  initRenderer(&renderer);
}

void loop() {
}
