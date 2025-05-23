#ifndef EMULATOR_H_
#define EMULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "SDL3/SDL.h"

#define WIDTH 1920
#define HEIGHT 1080
#define STACK_SIZE 16

typedef struct chip
{
	uint8_t keypad[0x10];
	uint8_t memory[4096];
	int PC;
	uint16_t stack[STACK_SIZE];
	float delay_timer;
	float sound_timer;
	uint8_t registers[16];
	int display[64][32];
	int end_of_mem;
	int IR;
} chip;

void handle_input(chip *emulator);
bool initialize_sdl(void);
void main_loop(chip *emulator);
chip *initialize_emulator(int argc, char **argv);
void check_valid_program(int argc, char **argv);
void load_sprite_data(chip *emulator);
void load_program(chip *emulator, char *filename);
uint16_t fetch_opcode(chip *emulator);
void execute(chip *emulator, uint16_t opcode);
void clear_display(chip *emulator);
void push(chip *emulator, uint16_t input);
uint16_t pop(chip *emulator);
void display(chip *emulator, int x_coord, int y_coord, int N);
void invalid_instruction(uint16_t opcode, chip *emulator);
void clear_keypad(chip *emulator);
void cleanup(chip *emulator);
void draw(chip *emulator);

#endif
