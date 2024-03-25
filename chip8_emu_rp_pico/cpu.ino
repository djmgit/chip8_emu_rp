#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "renderer.h"
#include "keyboard.h"
#include "cpu.h"

const uint8_t SPRITES[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const uint8_t ROM_CHIP8_LOGO[] = {
    0x0, 0xe0, 0x61, 0x1, 0x60, 
    0x8, 0xa2, 0x50, 0xd0, 0x1f,    
    0x60, 0x10, 0xa2, 0x5f, 0xd0,   
    0x1f, 0x60, 0x18, 0xa2, 0x6e,   
    0xd0, 0x1f, 0x60, 0x20, 0xa2,   
    0x7d, 0xd0, 0x1f, 0x60, 0x28,   
    0xa2, 0x8c, 0xd0, 0x1f, 0x60,   
    0x30, 0xa2, 0x9b, 0xd0, 0x1f,   
    0x61, 0x10, 0x60, 0x8, 0xa2,    
    0xaa, 0xd0, 0x1f, 0x60, 0x10,   
    0xa2, 0xb9, 0xd0, 0x1f, 0x60,   
    0x18, 0xa2, 0xc8, 0xd0, 0x1f,   
    0x60, 0x20, 0xa2, 0xd7, 0xd0,   
    0x1f, 0x60, 0x28, 0xa2, 0xe6,   
    0xd0, 0x1f, 0x60, 0x30, 0xa2,   
    0xf5, 0xd0, 0x1f, 0x12, 0x4e,   
    0xf, 0x2, 0x2, 0x2, 0x2,    
    0x2, 0x0, 0x0, 0x1f, 0x3f,  
    0x71, 0xe0, 0xe5, 0xe0, 0xe8,   
    0xa0, 0xd, 0x2a, 0x28, 0x28,    
    0x28, 0x0, 0x0, 0x18, 0xb8,     
    0xb8, 0x38, 0x38, 0x3f, 0xbf,   
    0x0, 0x19, 0xa5, 0xbd, 0xa1,    
    0x9d, 0x0, 0x0, 0xc, 0x1d,  
    0x1d, 0x1, 0xd, 0x1d, 0x9d,     
    0x1, 0xc7, 0x29, 0x29, 0x29,    
    0x27, 0x0, 0x0, 0xf8, 0xfc,     
    0xce, 0xc6, 0xc6, 0xc6, 0xc6,   
    0x0, 0x49, 0x4a, 0x49, 0x48,    
    0x3b, 0x0, 0x0, 0x0, 0x1,   
    0x3, 0x3, 0x3, 0x1, 0xf0,   
    0x30, 0x90, 0x0, 0x0, 0x80,     
    0x0, 0x0, 0x0, 0xfe, 0xc7,  
    0x83, 0x83, 0x83, 0xc6, 0xfc,   
    0xe7, 0xe0, 0xe0, 0xe0, 0xe0,   
    0x71, 0x3f, 0x1f, 0x0, 0x0,     
    0x7, 0x2, 0x2, 0x2, 0x2,    
    0x39, 0x38, 0x38, 0x38, 0x38,   
    0xb8, 0xb8, 0x38, 0x0, 0x0,     
    0x31, 0x4a, 0x79, 0x40, 0x3b,   
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd,   
    0xdd, 0xdd, 0xdd, 0x0, 0x0,     
    0xa0, 0x38, 0x20, 0xa0, 0x18,   
    0xce, 0xfc, 0xf8, 0xc0, 0xd4,   
    0xdc, 0xc4, 0xc5, 0x0, 0x0,     
    0x30, 0x44, 0x24, 0x14, 0x63,   
    0xf1, 0x3, 0x7, 0x7, 0x27,  
    0x67, 0x23, 0x71, 0x0, 0x0,     
    0x28, 0x8e, 0xa8, 0xa8, 0xa6,   
    0xce, 0x87, 0x3, 0x3, 0x3,  
    0x87, 0xfe, 0xfc, 0x0, 0x0,     
    0x60, 0x90, 0xf0, 0x80, 0x70
};

void initCPU(cpu_t *cpu) {
    cpu->i = 0;
    cpu->pc = 0x200;
    cpu->sp = -1;
    cpu->paused = 0;
    cpu->speed = 10;
    cpu->delayTimer = 0;
    cpu->soundTimer = 0;
    memset(cpu->memory, 0, MEMORY_SIZE);
    memset(cpu->registers, 0, NUM_REGISTERS);
    memset(cpu->stack, 0, STACK_SIZE);
    srand(time(0));

    loadSpritesIntoMemory(cpu);
}

void pushStack(cpu_t *cpu, uint16_t val) {
    cpu->sp = cpu->sp + 1;
    cpu->stack[cpu->sp] = val;
}

uint16_t popStack(cpu_t *cpu) {
    uint16_t val = cpu->stack[cpu->sp];
    cpu->sp = cpu->sp - 1;
    return val;
}

void loadSpritesIntoMemory(cpu_t *cpu) {
    for (size_t i = 0; i < NUM_FONT_SPRITES; i++) {
        cpu->memory[i] = SPRITES[i];
    }
}

void loadProgramIntoMemory(cpu_t *cpu) {
    for (size_t loc = 0; loc < sizeof(ROM_CHIP8_LOGO); loc++) {
        cpu->memory[0x200 + loc] = ROM_CHIP8_LOGO[loc];
    }
}

void updateTimers(cpu_t *cpu) {
    cpu->delayTimer = cpu->delayTimer > 0 ? cpu->delayTimer - 1 : cpu->delayTimer;
    cpu->soundTimer = cpu->soundTimer > 0 ? cpu->soundTimer - 1 : cpu->soundTimer;
}

void cycle(cpu_t *cpu, keyboard_t *keyboard, renderer_t *renderer) {
    for (size_t i = 0; i < cpu->speed; i++) {
        if (!cpu->paused) {
            uint16_t instruction = (cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1]);
            executeInstruction(cpu, renderer, keyboard, instruction);
        }
    }

    if (!cpu->paused) {
        updateTimers(cpu);
    }
}

void executeInstruction(cpu_t *cpu, renderer_t *renderer, keyboard_t *keyboard, uint16_t instruction) {
    cpu->pc += 2;

    uint8_t x = (instruction & 0x0f00) >> 8;
    uint8_t y = (instruction & 0x00f0) >> 4;

    switch(instruction & 0xf000) {
        case 0x0000:
            switch(instruction) {
                case 0x00e0:
                    clear(renderer);
                    break;
                case 0x00ee:
                    cpu->pc = popStack(cpu);
                    break;
            }
            break;
        case 0x1000:
            cpu->pc = (instruction & 0xfff);
            break;
        
        case 0x2000:
            pushStack(cpu, cpu->pc);
            cpu->pc = (instruction & 0xfff);
            break;
        
        case 0x3000:
            if (cpu->registers[x] == (uint8_t)(instruction & 0xff)) {
                cpu->pc += 2;
            }
            break;
        
        case 0x4000:
            if (cpu->registers[x] != (uint8_t)(instruction & 0xff)) {
                cpu->pc += 2;
            }
            break;
        
        case 0x5000:
            if (cpu->registers[x] == cpu->registers[y]) {
                cpu->pc += 2;
            }
            break;
        
        case 0x6000:
            cpu->registers[x] = (uint8_t)(instruction & 0xff);
            break;
        
        case 0x7000:
            cpu->registers[x] += (uint8_t)(instruction & 0xff);
            break;
        
        case 0x8000:
            switch(instruction & 0xf){
                case 0x0:
                    cpu->registers[x] = cpu->registers[y];
                    break;
                
                case 0x1:
                    cpu->registers[x] |= cpu->registers[y];
                    break;
                
                case 0x2:
                    cpu->registers[x] &= cpu->registers[y];
                    break;
                
                case 0x3:
                    cpu->registers[x] ^= cpu->registers[y];
                    break;
                
                case 0x4:
                    {
                        uint16_t sum = (cpu->registers[x] += cpu->registers[y]);
                        cpu->registers[0xf] = 0;
                        if (sum > 0xff) {
                            cpu->registers[0xf] = 1;
                        }
                        cpu->registers[x] = (uint8_t)(sum & 0xff);
                        break;
                    }
                
                case 0x5:
                   cpu->registers[0xf] = 0;
                   if (cpu->registers[x] > cpu->registers[y]) {
                    cpu->registers[0xf] = 1;
                   }
                   cpu->registers[x] -= cpu->registers[y];
                   break;
                
                case 0x6:
                    cpu->registers[0xf] = (cpu->registers[x] & 0x1);
                    cpu->registers[x] >>= 1;
                    break;
                
                case 0x7:
                    cpu->registers[0xf] = 0;
                    
                    if (cpu->registers[y] > cpu->registers[x]) {
                        cpu->registers[0xf] = 1;
                    }

                    cpu->registers[x] = cpu->registers[y] - cpu->registers[x];
                    break;
                
                case 0xe:
                    cpu->registers[0xf] = (cpu->registers[x] >> 7);
                    cpu->registers[x] <<= 1;
                    break;
            }
            break;
        
        case 0x9000:
            if (cpu->registers[x] != cpu->registers[y]) {
                cpu->pc += 2;
            }
            break;
        
        case 0xa000:
            cpu->i = (instruction & 0xfff);
            break;
        
        case 0xb000:
            cpu->pc = (cpu->registers[0x0] + (instruction & 0xfff));
            break;
        
        case 0xc000:
            {
                uint8_t random_num = rand() % 256;
                cpu->registers[x] = (random_num & (instruction & 0xff));
                break;
            }
        
        case 0xd000:
            {
                uint8_t height = (instruction & 0xf);
                cpu->registers[0xf] = 0;

                for (size_t row = 0; row < height; row++) {
                    uint8_t sprite_row = cpu->memory[cpu->i + (uint16_t)row];
                    for (size_t col = 0; col < SPRITE_WIDTH; col++) {
                        if ((sprite_row & 0x80) > 0) {
                            if (setPixel(renderer, cpu->registers[x] + col, cpu->registers[y] + row)) {
                                cpu->registers[0xf] = 1;
                            }
                        }
                        sprite_row <<= 1;
                    }
                }
                break;
            }
        
        case 0xe000:
            switch(instruction & 0xff) {
                case 0x9e:
                    if (isKeyPressed(keyboard, cpu->registers[x])) {
                        cpu->pc += 2;
                    }
                    break;
                
                case 0xa1:
                    if (!isKeyPressed(keyboard, cpu->registers[x])) {
                        cpu->pc += 2;
                    }
                    break;
            }
            break;
        
        case 0xf000:
            switch(instruction & 0xff) {
                case 0x07:
                    cpu->registers[x] = cpu->delayTimer;
                    break;
                
                case 0x0a:
                    cpu->paused = 1;
                    cpu->key_register = x;
                    keyboard->onNextKeyPress = onNextKeyPress;
                    break;
                
                case 0x15:
                    cpu->delayTimer = cpu->registers[x];
                    break;
                
                case 0x18:
                    cpu->soundTimer = cpu->registers[x];
                    break;
                
                case 0x1E:
                    cpu->i += cpu->registers[x];
                    break;
                
                case 0x29:
                    cpu->i = cpu->registers[x] * 5;
                    break;
                
                case 0x33:
                    cpu->memory[cpu->i] = (uint8_t)(cpu->registers[x] / 100);
                    cpu->memory[cpu->i + 1] = (uint8_t)((cpu->registers[x] % 100) / 10);
                    cpu->memory[cpu->i + 2] = (uint8_t)(cpu->registers[x] % 10);
                    break;
                
                case 0x55:
                    for (size_t registerIndex = 0; registerIndex <= x; registerIndex++) {
                        cpu->memory[cpu->i + registerIndex] = cpu->registers[registerIndex];
                    }
                    break;
                
                case 0x65:
                    for (size_t registerIndex = 0; registerIndex <= x; registerIndex++) {
                        cpu->registers[registerIndex] = cpu->memory[cpu->i + registerIndex];
                    }
                    break;
            }
            break;
        default:
            assert (1==0);
    }
}

