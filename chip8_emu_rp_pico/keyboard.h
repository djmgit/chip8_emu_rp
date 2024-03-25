#ifndef KEYBOARD_H
#define KEYBOARD_H
#define NUM_VALID_KEYS 16
#define KEY_ARRAY_MAX 91

#include <stdint.h>
#include "cpu.h"

typedef struct cpu_t cpu_t;
typedef struct keyboard_t {
    uint8_t keymap[KEY_ARRAY_MAX];
    uint8_t keyPressed[17];
    uint8_t validKeys[17];
    void (* onNextKeyPress) (cpu_t *cpu, uint8_t);
} keyboard_t;

void initKeyBoard(keyboard_t *keyboard);
void onNextKeyPress(cpu_t *cpu, uint8_t);
int isKeyValid(keyboard_t *keyboard, uint8_t);
void onKeyUp(keyboard_t *keyboard, uint8_t);
void onKeyDown(keyboard_t *keyboard, cpu_t* cpu, uint8_t);
int isKeyPressed(keyboard_t *keyboard, uint8_t);
void processKey(keyboard_t *keyboard, cpu_t *cpu);

#endif
