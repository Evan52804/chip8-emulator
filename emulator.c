#include "emulator.h"

// Program parameters
bool DEBUG = 0;
bool ORIGINAL_SHIFT = 0;
bool ORIGINAL_JUMP_OFFSET = 1;
bool ORIGINAL_STORE_LOAD = 0;
bool PAUSE = 0;
uint RATE = 700;
int FPS = 60;


// SDL components
SDL_Window *window;
SDL_Renderer *renderer;

int main(int argc, char **argv)
{
	chip *emulator = initialize_emulator(argc, argv);

	printf("\n\n==============\n   Controls   \n==============\n\nF1: Toggle debug mode\nF10: Toggle shift behavior\n");
	printf("F11: Toggle jump with offset behavior\nF12:Toggle store / load behavior\nEnter: Reset\nSpace: Pause\n");
	printf("Equals: Increase speed\nMinus: Decrease speed\n\n");

	if (DEBUG)
		printf("DEBUG: Emulator initialized, beginning main emulation loop\n\n");

	main_loop(emulator);

	cleanup(emulator);

	return 0;
}

void cleanup(chip *emulator)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(emulator);
}

void main_loop(chip *emulator)
{
	uint64_t time_before;
	uint64_t time_after;
	double elapsed;

	while ((emulator->PC != emulator->end_of_mem))
	{
		time_before = SDL_GetPerformanceCounter();

		handle_input(emulator);

		for (uint i = 0; i < RATE / FPS && !PAUSE && emulator->PC < emulator->end_of_mem; ++i)
        {
		    handle_input(emulator);
			execute(emulator, fetch_opcode(emulator));
        }

		if (!PAUSE)
			draw(emulator);

		time_after = SDL_GetPerformanceCounter();
		elapsed = (double)((time_after - time_before) / 1000.0) / SDL_GetPerformanceFrequency();

		if (elapsed < 1.0 / FPS)
			SDL_Delay((1.0 / FPS) * 1000 - elapsed);

		if (emulator->delay_timer > 0 && !PAUSE)
			emulator->delay_timer -= 1;

		if (emulator->sound_timer > 0 && !PAUSE)
			emulator->sound_timer -= 1;
	}
}

void handle_input(chip *emulator)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_EVENT_QUIT:

				emulator->PC = emulator->end_of_mem;
				
				break;

			case SDL_EVENT_KEY_DOWN:

				switch (event.key.key)
				{
					case SDLK_ESCAPE:

						emulator->PC = emulator->end_of_mem;

						break;

					case SDLK_1:

						emulator->keypad[0x1] = 1;

						break;

					case SDLK_2:

						emulator->keypad[0x2] = 1;

						break;

					case SDLK_3:

						emulator->keypad[0x3] = 1;

						break;

					case SDLK_Q:

						emulator->keypad[0x4] = 1;

						break;

					case SDLK_W:

						emulator->keypad[0x5] = 1;

						break;

					case SDLK_E:

						emulator->keypad[0x6] = 1;

						break;

					case SDLK_A:

						emulator->keypad[0x7] = 1;

						break;

					case SDLK_S:

						emulator->keypad[0x8] = 1;

						break;

					case SDLK_D:

						emulator->keypad[0x9] = 1;

						break;

					case SDLK_X:

						emulator->keypad[0x0] = 1;

						break;

					case SDLK_Z:

						emulator->keypad[0xA] = 1;

						break;

					case SDLK_C:

						emulator->keypad[0xB] = 1;

						break;

					case SDLK_4:

						emulator->keypad[0xC] = 1;

						break;

					case SDLK_R:

						emulator->keypad[0xD] = 1;

						break;

					case SDLK_F:

						emulator->keypad[0xE] = 1;

						break;

					case SDLK_V:

						emulator->keypad[0xF] = 1;

						break;

					case SDLK_F1:

						DEBUG = !DEBUG;

						break;

					case SDLK_F10:

						ORIGINAL_SHIFT = !ORIGINAL_SHIFT;
						printf("Original Shift: %d\n", ORIGINAL_SHIFT);

						break;

					case SDLK_F11:

						ORIGINAL_JUMP_OFFSET = !ORIGINAL_JUMP_OFFSET;
						printf("Original Jump Offset: %d\n", ORIGINAL_JUMP_OFFSET);

						break;

					case SDLK_F12:

						ORIGINAL_STORE_LOAD = !ORIGINAL_STORE_LOAD;
						printf("Original Store/Load: %d\n", ORIGINAL_STORE_LOAD);

						break;

					case SDLK_END:

						clear_display(emulator);
						SDL_RenderPresent(renderer);
						emulator->PC = 0x200;

						break;

					case SDLK_SPACE:

						PAUSE = !PAUSE;

						if (PAUSE)
							printf("\n========\n PAUSED \n========\n\n");

						break;

					case SDLK_EQUALS:

						RATE += 50;

						printf("Clock speed: %d\n", RATE);

						break;

					case SDLK_MINUS:

						if (RATE > 0)
						RATE -= 50;

						printf("Clock speed: %d\n", RATE);

						break;

					default:

						break;
				}

				break;

			case SDL_EVENT_KEY_UP:

				switch (event.key.key)
				{
					case SDLK_1:

						emulator->keypad[0x1] = 0;

						break;

					case SDLK_2:

						emulator->keypad[0x2] = 0;

						break;

					case SDLK_3:

						emulator->keypad[0x3] = 0;

						break;

					case SDLK_Q:

						emulator->keypad[0x4] = 0;

						break;

					case SDLK_W:

						emulator->keypad[0x5] = 0;

						break;

					case SDLK_E:

						emulator->keypad[0x6] = 0;

						break;

					case SDLK_A:

						emulator->keypad[0x7] = 0;

						break;

					case SDLK_S:

						emulator->keypad[0x8] = 0;

						break;

					case SDLK_D:

						emulator->keypad[0x9] = 0;

						break;

					case SDLK_X:

						emulator->keypad[0x0] = 0;

						break;

					case SDLK_Z:

						emulator->keypad[0xA] = 0;

						break;

					case SDLK_C:

						emulator->keypad[0xB] = 0;

						break;

					case SDLK_4:

						emulator->keypad[0xC] = 0;

						break;

					case SDLK_R:

						emulator->keypad[0xD] = 0;

						break;

					case SDLK_F:

						emulator->keypad[0xE] = 0;

						break;

					case SDLK_V:

						emulator->keypad[0xF] = 0;

						break;

					default:

						break;
				}

				break;

			default:

				break;
		}
	}
}

bool initialize_sdl(void)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Error: SDL failed to initialize\n");
		return 0;
	}

	window = SDL_CreateWindow("Chip-8 Emulator", WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, 0);

	if (DEBUG)
		printf("DEBUG: Initialized SDL\n");

	return 1;
}

chip *initialize_emulator(int argc, char **argv)
{
	check_valid_program(argc, argv);
	srand(time(NULL));

	chip *emulator = malloc(sizeof(chip));

	if (emulator == NULL)
	{
		printf("Error: Failed to allocate memory\n");
		exit(1);
	}

	emulator->PC = 0x200;
	emulator->end_of_mem = 0x200;

	clear_keypad(emulator);
	load_sprite_data(emulator);
	load_program(emulator, argv[1]);

	if (!initialize_sdl())
	{
		cleanup(emulator);
		exit(1);
	}

	return emulator;
}

void check_valid_program(int argc, char **argv)
{
	// Ensure that the user specifies one input file, and optionally, the debug flag
	if (argc < 2)
	{
		printf("Error: No input file.\n");
		exit(1);
	}
	else if (argc == 3)
	{
		DEBUG = argv[2][0] - '0';
		
		printf("Debug enabled.\n");
	}
	else if (argc > 3)
	{
		printf("Error: Too many inputs\n");
		exit(1);
	}

	if (DEBUG)
		printf("DEBUG: Program input is valid\n");
}

void load_sprite_data(chip *emulator)	// Convention dictates that fonts are stored at 0x50-0x9F
{
	// 0
	emulator->memory[0x50] = 0xF0;
	emulator->memory[0x51] = 0x90;
	emulator->memory[0x52] = 0x90;
	emulator->memory[0x53] = 0x90;
	emulator->memory[0x54] = 0xF0;

	// 1
	emulator->memory[0x55] = 0x20;
	emulator->memory[0x56] = 0x60;
	emulator->memory[0x57] = 0x20;
	emulator->memory[0x58] = 0x20;
	emulator->memory[0x59] = 0x70;

	// 2
	emulator->memory[0x5A] = 0xF0;
	emulator->memory[0x5B] = 0x10;
	emulator->memory[0x5C] = 0xF0;
	emulator->memory[0x5D] = 0x80;
	emulator->memory[0x5E] = 0xF0;

	// 3
	emulator->memory[0x5F] = 0xF0;
	emulator->memory[0x60] = 0x10;
	emulator->memory[0x61] = 0xF0;
	emulator->memory[0x62] = 0x10;
	emulator->memory[0x63] = 0xF0;

	// 4
	emulator->memory[0x64] = 0x90;
	emulator->memory[0x65] = 0x90;
	emulator->memory[0x66] = 0xF0;
	emulator->memory[0x67] = 0x10;
	emulator->memory[0x68] = 0x10;

	// 5
	emulator->memory[0x69] = 0xF0;
	emulator->memory[0x6A] = 0x80;
	emulator->memory[0x6B] = 0xF0;
	emulator->memory[0x6C] = 0x10;
	emulator->memory[0x6D] = 0xF0;

	// 6
	emulator->memory[0x6E] = 0xF0;
	emulator->memory[0x6F] = 0x80;
	emulator->memory[0x70] = 0xF0;
	emulator->memory[0x71] = 0x90;
	emulator->memory[0x72] = 0xF0;

	// 7
	emulator->memory[0x73] = 0xF0;
	emulator->memory[0x74] = 0x10;
	emulator->memory[0x75] = 0x20;
	emulator->memory[0x76] = 0x40;
	emulator->memory[0x77] = 0x40;

	// 8
	emulator->memory[0x78] = 0xF0;
	emulator->memory[0x79] = 0x90;
	emulator->memory[0x7A] = 0xF0;
	emulator->memory[0x7B] = 0x90;
	emulator->memory[0x7C] = 0xF0;

	// 9
	emulator->memory[0x7D] = 0xF0;
	emulator->memory[0x7E] = 0x90;
	emulator->memory[0x7F] = 0xF0;
	emulator->memory[0x80] = 0x10;
	emulator->memory[0x81] = 0xF0;

	// A
	emulator->memory[0x82] = 0xF0;
	emulator->memory[0x83] = 0x90;
	emulator->memory[0x84] = 0xF0;
	emulator->memory[0x85] = 0x90;
	emulator->memory[0x86] = 0x90;

	// B
	emulator->memory[0x87] = 0xE0;
	emulator->memory[0x88] = 0x90;
	emulator->memory[0x89] = 0xE0;
	emulator->memory[0x8A] = 0x90;
	emulator->memory[0x8B] = 0xE0;

	// C
	emulator->memory[0x8C] = 0xF0;
	emulator->memory[0x8D] = 0x80;
	emulator->memory[0x8E] = 0x80;
	emulator->memory[0x8F] = 0x80;
	emulator->memory[0x90] = 0xF0;

	// D
	emulator->memory[0x91] = 0xE0;
	emulator->memory[0x92] = 0x90;
	emulator->memory[0x93] = 0x90;
	emulator->memory[0x94] = 0x90;
	emulator->memory[0x95] = 0xE0;

	// E
	emulator->memory[0x96] = 0xF0;
	emulator->memory[0x97] = 0x80;
	emulator->memory[0x98] = 0xF0;
	emulator->memory[0x99] = 0x80;
	emulator->memory[0x9A] = 0xF0;

	// F
	emulator->memory[0x9B] = 0xF0;
	emulator->memory[0x9C] = 0x80;
	emulator->memory[0x9D] = 0xF0;
	emulator->memory[0x9E] = 0x80;
	emulator->memory[0x9F] = 0x80;

	if (DEBUG)
		printf("DEBUG: Loaded sprite data\n");
}

void load_program(chip *emulator, char *filename)	// Convention dictates that programs are loaded into memory at address 0x200
{
	FILE *file_ptr = fopen(filename, "rb");
	uint8_t temp;

	if (file_ptr == NULL)
	{
		printf("Error: Failed to open file '%s'\n", filename);
		cleanup(emulator);
		exit(1);
	}
	
	if (DEBUG)
		printf("DEBUG: File '%s' opened Loading program\n", filename);

	while (fread(&temp, sizeof(uint8_t), 1, file_ptr) == 1 && emulator->end_of_mem <= 4097)
	{
		emulator->memory[emulator->end_of_mem] = temp;
		
		if (DEBUG)
			printf("DEBUG: Stored byte [%02x] at address [0x%03x]\n", emulator->memory[emulator->end_of_mem], emulator->end_of_mem);

		(emulator->end_of_mem)++;
	}

	if (emulator->end_of_mem > 4097)
	{
		printf("Error: Program exceeds memory limit\n");
		cleanup(emulator);
		exit(1);
	}

	if (DEBUG)
		printf("DEBUG: Program loaded into memory\n");

	fclose(file_ptr);
}

uint16_t fetch_opcode(chip *emulator)
{
	if (emulator->PC > 4096)
	{
		printf("Error: Accessing restricted memory\n");
		cleanup(emulator);
		exit(1);
	}

	int first_byte = emulator->memory[emulator->PC];
	int second_byte = emulator->memory[emulator->PC + 1];

	emulator->PC += 2;

	if (DEBUG)
		printf("DEBUG: Fetched instruction [0x%04x] from address [0x%03x]\n", first_byte << 8 | second_byte, emulator->PC - 2);

	return first_byte << 8 | second_byte;
}

void execute(chip *emulator, uint16_t opcode)
{
	uint X = (opcode & 0x0F00) >> 8;
	uint Y = (opcode & 0x00F0) >> 4;
	uint N = opcode & 0x000F;
	uint NN = opcode & 0x00FF;
	uint NNN = opcode & 0x0FFF;

	uint8_t VX = emulator->registers[X];
	uint8_t VY = emulator->registers[Y];

	switch (opcode & 0xF000)
	{
		case 0x0000:
			
			switch (opcode & 0x000F)				// Clear display
			{
				case 0x0000:

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Clear display");

					clear_display(emulator);

					break;

				case 0x000E:						// Return from subroutine

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Return from subroutine");

					emulator->PC = pop(emulator);

					break;

				default:

					invalid_instruction(opcode, emulator);

					break;
			}

			break;

		case 0x1000:								// Jump

			if (DEBUG)
					printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Jump");

			emulator->PC = NNN;

			break;

		case 0x2000:								// Call subroutine

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Call subroutine");

			push(emulator, emulator->PC);
			emulator->PC = NNN;

			break;

		case 0x3000:								// Conditional skip

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Conditional skip");

			if (VX == NN)
				emulator->PC += 2;

			break;

		case 0x4000:								// Conditional skip

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Conditional skip");

			if (VX != NN)
				emulator->PC += 2;

			break;

		case 0x5000:								// Conditional skip

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Conditional skip");

			if (VX == VY)
				emulator->PC += 2;

			break;

		case 0x6000:								// Set

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Set");

			emulator->registers[X] = NN;

			break;

		case 0x7000:								// Add

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Add");

			emulator->registers[X] += NN;

			break;

		case 0x8000:

			switch (opcode & 0x000F)
			{
				case 0x0000:							// Set

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Set");

					emulator->registers[X] = VY;

					break;

				case 0x0001:							// OR

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "OR");

					emulator->registers[X] |= VY;

					break;

				case 0x0002:							// AND

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "AND");

					emulator->registers[X] &= VY;

					break;

				case 0x0003:							// XOR

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "XOR");

					emulator->registers[X] ^= VY;

					break;

				case 0x0004:							// Add

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Add");

					emulator->registers[0xF] = 0;

					emulator->registers[X] = VX + VY;
					
					if ((int)VX + (int)VY > 255)
						emulator->registers[0xF] = 1;

					break;

				case 0x0005:							// Subtract

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Subtract");

					emulator->registers[0xF] = 1;

					emulator->registers[X] -= VY;

					if (VX < VY)
						emulator->registers[0xF] = 0;

					break;

				case 0x0006:							// Shift right

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Shift right");

					if (ORIGINAL_SHIFT)
						VX = VY;

					emulator->registers[0xF] = VX & 1;
					emulator->registers[X] >>= 1;

					break;

				case 0x0007:							// Subtract

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Subtract");

					emulator->registers[0xF] = 1;

					emulator->registers[X] = VY - VX;

					if (VY < VX)
						emulator->registers[0xF] = 0;

					break;

				case 0x000E:							// Shift left

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Shift left");

					if (ORIGINAL_SHIFT)
						VX = VY;

					emulator->registers[0xF] = (VX >> 7) & 1;
					emulator->registers[X] <<= 1;

					break;

				default:

					invalid_instruction(opcode, emulator);

					break;
			}

			break;

		case 0x9000:								// Conditional skip

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Conditional skip");

			if (VX != VY)
				emulator->PC += 2;

			break;

		case 0xA000:								// Set index register

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Set index register");

			emulator->IR = NNN;

			break;

		case 0xB000:								// Jump with offset

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Jump with offset");

			if (ORIGINAL_JUMP_OFFSET)
				emulator->PC = NNN + emulator->registers[0];
			else
				emulator->PC = ((X << 8) | NN) + VX;

			break;

		case 0xC000:								// Random

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Random");

			emulator->registers[X] = (rand() % 256) & NN;

			if (DEBUG)
				printf("DEBUG: Random value [%x] generated.\n", emulator->registers[X]);

			break;

		case 0xD000:								// Display

			if (DEBUG)
				printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Display");

			display(emulator, VX % 64, VY % 32, N);

			break;

		case 0xE000:

			switch (opcode & 0x00FF)
			{
				case 0x009E:						// Skip if key

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Skip if key");

					if (emulator->keypad[VX])
						emulator->PC += 2;

					break;

				case 0x00A1:						// Skip if NOT key

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Skip if NOT key");

					if (!emulator->keypad[VX])
						emulator->PC += 2;

					break;

				default:

					invalid_instruction(opcode, emulator);

					break;
			}

			break;

		case 0xF000:

			switch (opcode & 0x00FF)
			{
				case 0x0007:						// Set VX to delay timer

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Delay timer to register");

					emulator->registers[X] = emulator->delay_timer;

					break;

				case 0x0015:						// Set delay timer to VX

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Register to delay timer");

					emulator->delay_timer = VX;

					break;

				case 0x0018:						// Set sound timer to VX

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Sound timer to register");

					emulator->sound_timer = VX;

					break;

				case 0x001E:						// Add VX to IR

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Add register to index");

					emulator->registers[0xF] = 0;

					if (emulator->IR + VX > 0x0FFF)
						emulator->registers[0xF] = 1;

					emulator->IR += VX;

					break;

				case 0x000A:						// Get input

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Wait for input");

					for (int i = 0; i < 16; ++i)
					{
						if (emulator->keypad[i])
						{
							emulator->registers[X] = i;
							return;
						}
					}

					emulator->PC -= 2;

					break;

				case 0x0029:						// Font character

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Font character");

					VX = VX & 0x000F;
					emulator->IR = 0x50 + (5 * VX);

					break;

				case 0x0033:						// Binary-coded decimal conversion

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Binary-coded decimal conversion");

					if (emulator->IR > 4096)
					{
						printf("Error: IR [%x] attempts to access restricted memory.\n", emulator->IR);
						cleanup(emulator);
						exit(1);
					}

					emulator->memory[emulator->IR] = VX / 100;
					emulator->memory[emulator->IR + 1] = (VX / 10) % 10;
					emulator->memory[emulator->IR + 2] = VX % 10;

					break;

				case 0x0055:						// Store memory

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Store memory");

					if (emulator->IR > 4096)
					{
						printf("Error: IR [%x] attempts to access restricted memory.\n", emulator->IR);
						cleanup(emulator);
						exit(1);
					}

					for (int i = 0; i <= (int)X; ++i)
					{
						emulator->memory[emulator->IR + i] = emulator->registers[i];

						if (ORIGINAL_STORE_LOAD)
							emulator->IR += 1;
					}

					break;

				case 0x0065:						// Load memory

					if (DEBUG)
						printf("DEBUG: Executing operation [0x%x]: '%s'\n\n", opcode, "Load memory");

					if (emulator->IR > 4096)
					{
						printf("Error: IR [%x] attempts to access restricted memory.\n", emulator->IR);
						cleanup(emulator);
						exit(1);
					}

					for (int i = 0; i <= (int)X; ++i)
					{
						emulator->registers[i] = emulator->memory[emulator->IR + i];

						if (ORIGINAL_STORE_LOAD)
							emulator->IR += 1;
					}

					break;

				default:

					invalid_instruction(opcode, emulator);

					break;
			}

			break;

		default:

			invalid_instruction(opcode, emulator);

			break;
	}
}

void clear_display(chip *emulator)
{
	for (int i = 0; i < 64; ++i)
		for (int j = 0; j < 32; ++j)
			emulator->display[i][j] = 0;

	draw(emulator);
}

void push(chip *emulator, uint16_t input)
{
	for (int i = STACK_SIZE; i > 0; --i)
		emulator->stack[i] = emulator->stack[i - 1];

	emulator->stack[0] = input;
}

uint16_t pop(chip *emulator)
{
	uint16_t retval = emulator->stack[0];

	for (int i = 0; i < STACK_SIZE; ++i)
		emulator->stack[i] = emulator->stack[i + 1];

	return retval;
}

void display(chip *emulator, int x_coord, int y_coord, int N)
{
	if (emulator->IR > 4096)
	{
		printf("Error: IR [%x] attempts to access restricted memory\n", emulator->IR);
		cleanup(emulator);
		exit(1);
	}

	emulator->registers[0xF] = 0;

	uint8_t row;
	int sprite_bit;
	int screen_bit = emulator->display[x_coord][y_coord];

	for (int i = 0; i < N; ++i)
	{
		if (y_coord + i >= 32)
			break;

		row = emulator->memory[emulator->IR + i];

		for (int j = 0; j < 8; ++j)
		{
			if (x_coord + j >= 64)
				break;

			screen_bit = emulator->display[x_coord + j][y_coord + i];

			sprite_bit = (row >> (7 - j)) & 1;

			if (sprite_bit && screen_bit)
			{
				emulator->registers[0xF] = 1;
				emulator->display[x_coord + j][y_coord + i] = 0;
			}
			else if (sprite_bit)
			{
				emulator->display[x_coord + j][y_coord + i] = 1;
			}
		}
	}
}

void draw(chip *emulator)
{
	SDL_FRect square;
	square.w = (WIDTH / 64) - 1;
	square.h = (HEIGHT / 32) - 1;

	// This solution is far from efficient, but it eliminates any issues with flickering or buffers
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			square.x = (i * (WIDTH / 64));
			square.y = (j * (HEIGHT / 32));

			if (emulator->display[i][j])
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_RenderFillRect(renderer, &square);
		}
	}
	SDL_RenderPresent(renderer);
}

void invalid_instruction(uint16_t opcode, chip *emulator)
{
	printf("Error: Invalid instruction [0x%x] encountered\n", opcode);
	cleanup(emulator);
	exit(1);
}

void clear_keypad(chip *emulator)
{
	for (int i = 0; i < 16; ++i)
		emulator->keypad[i] = 0;
}
