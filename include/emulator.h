#pragma once
#include "stdint.h"

#define mem_size 4096

class Emulator {
public:
	// Memory 4KB
	uint8_t memory[mem_size];
	// The Chip 8 specification states that the first 512 ($200 hex) bytes of memory are reserved for the interpreter
	// and should not be used as general storage.
	// Within that 512 byte reserved space, the first 80 ($50 hex) bytes contain sprite information for the characters 0 through 9 and A through F. 
	// Sprite information from the starting memory address memory = 0
	
	// 15 8-bit general purpose registers named V0,V1 up to VE.
	// The 16th register is used for the �carry flag�.
	uint8_t reg[16];
	// Index register I and a program counter(PC) which can have a value from 0x000 to 0xFFF
	uint16_t I,PC;
	// The graphics of the Chip 8 has a total of 2048 pixels (64 x 32).
	bool graphics[32*64]; // Width = 64 and Height = 32;
	// Two 8 bit timer registers that count at 60 Hz.
	// When set above zero they will count down to zero.
	uint8_t delay_timer;
	// The system�s buzzer sounds whenever the sound timer reaches zero.
	uint8_t sound_timer;
	// The system has 16 levels of stack
	uint16_t stack[16];
	// Stack pointer holds the offset value from the initial address stack
	uint8_t SP = 0;
	// Chip 8 has a HEX based keypad(0x0 - 0xF)
	uint8_t key[16];

	void initialize();
	void emulateCycle();
};
