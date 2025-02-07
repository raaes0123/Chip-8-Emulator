#include "./include/emulator.h"
#include "cstring"
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <map>
#include<iostream>
using namespace std;

void Emulator::initialize() {
	PC = 0x200;  // Program counter starts at 0x200
	// Clear the memory
	memset(memory, 0, sizeof(memory));
	// Clear the graphics memory
	memset(graphics, false, sizeof(graphics));
	// Clear the registers
	memset(reg, 0, sizeof(reg));
	I = 0;
	// Clear stack
	memset(stack, 0, sizeof(stack));
	SP = 0;
	// Reset timers
	// Seed random number
	srand(unsigned(time(0)));
	// Sprite information of hex numbers from 0 to F starting at memory address memory = 0
	// Sprite = 0 Location: 0 to 4
	memory[0] = 0xF0;
	memory[1] = 0x90;
	memory[2] = 0x90;
	memory[3] = 0x90;
	memory[4] = 0xF0;
	// Sprite = 1 Location: 5 to 9
	memory[5] = 0x20;
	memory[6] = 0x60;
	memory[7] = 0x20;
	memory[8] = 0x20;
	memory[9] = 0x70;
	// Sprite = 2 Location: 10 to 14
	memory[10] = 0xF0;
	memory[11] = 0x10;
	memory[12] = 0xF0;
	memory[13] = 0x80;
	memory[14] = 0xF0;
	// Sprite = 3 Location: 15 to 19
	memory[15] = 0xF0;
	memory[16] = 0x10;
	memory[17] = 0xF0;
	memory[18] = 0x10;
	memory[19] = 0xF0;
	// Sprite = 4 Location: 20 to 24
	memory[20] = 0x90;
	memory[21] = 0x90;
	memory[22] = 0xF0;
	memory[23] = 0x10;
	memory[24] = 0x10;
	// Sprite = 5 Location: 25 to 29
	memory[25] = 0xF0;
	memory[26] = 0x80;
	memory[27] = 0xF0;
	memory[28] = 0x10;
	memory[29] = 0xF0;
	// Sprite = 6 Location: 30 to 34
	memory[30] = 0xF0;
	memory[31] = 0x80;
	memory[32] = 0xF0;
	memory[33] = 0x90;
	memory[34] = 0xF0;
	// Sprite = 7 Location: 35 to 39
	memory[35] = 0xF0;
	memory[36] = 0x10;
	memory[37] = 0x20;
	memory[38] = 0x40;
	memory[39] = 0x40;
	// Sprite = 8 Location: 40 to 44
	memory[40] = 0xF0;
	memory[41] = 0x90;
	memory[42] = 0xF0;
	memory[43] = 0x90;
	memory[44] = 0xF0;
	// Sprite = 9 Location: 45 to 49
	memory[45] = 0xF0;
	memory[46] = 0x90;
	memory[47] = 0xF0;
	memory[48] = 0x10;
	memory[49] = 0xF0;
	// Sprite = A Location: 50 to 54
	memory[50] = 0xF0;
	memory[51] = 0x90;
	memory[52] = 0xF0;
	memory[53] = 0x90;
	memory[54] = 0x90;
	// Sprite = B Location: 55 to 59
	memory[55] = 0xE0;
	memory[56] = 0x90;
	memory[57] = 0xE0;
	memory[58] = 0x90;
	memory[59] = 0xE0;
	// Sprite = C Location: 60 to 64
	memory[60] = 0xF0;
	memory[61] = 0x80;
	memory[62] = 0x80;
	memory[63] = 0x80;
	memory[64] = 0xF0;
	// Sprite = D Location: 65 to 69
	memory[65] = 0xE0;
	memory[66] = 0x90;
	memory[67] = 0x90;
	memory[68] = 0x90;
	memory[69] = 0xE0;
	// Sprite = E Location: 70 to 74
	memory[70] = 0xF0;
	memory[71] = 0x80;
	memory[72] = 0xF0;
	memory[73] = 0x80;
	memory[74] = 0xF0;
	// Sprite = F Location: 75 to 79
	memory[75] = 0xF0;
	memory[76] = 0x80;
	memory[77] = 0xF0;
	memory[78] = 0x80;
	memory[79] = 0x80;
}
void Emulator::emulateCycle()
{
	// Fetch Opcode from memory
	uint8_t byte_high = memory[PC];
	uint8_t byte_low = memory[PC + 1];
	uint16_t opcode = byte_high << 8 | byte_low;
	//cout << hex << int(PC) << ": ";
	//cout << hex << int(byte_high) << " " << hex << int(byte_low) << endl;
	// Every instruction is 2 bytes long
	PC += 2;
	// Decode and Execute Opcode
	//cout << "----------Ball_y: " << int(reg[7]) << endl;
	//cout << "----------Padd_y: " << int(reg[0xB]) << endl;
	switch (opcode & 0xF000)
	{
	case(0x0000):
		switch (opcode & 0x00FF)
		{
		case(0x00E0): //0x00E0
			// CLS
			memset(graphics, false, sizeof(graphics));
			break;
		case(0x00EE): //0x00EE
			// RET
			SP--;
			PC = stack[SP];
			PC += 2;
			break;
		default:	  //0xnnn
			// Jump to native assembler subroutine at 0xNNN 
			// It is ignored by modern interpreters.
			break;
		}
		break;
	case(0x1000): //0x1nnn
		// Jump to address nnn
		PC = opcode & 0x0FFF;
		break;
	case(0x2000): //0x2nnn
		// Push return address onto stack
		// And call subroutine at address NNN
		stack[SP] = PC - 2;		// PC is increased by 2 above so current PC should be PC-2
		PC = opcode & 0x0FFF;
		SP++;
		break;
	case(0x3000): //0x3xkk
	{
		// Skip next instruction if Vx = kk.
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t k = (opcode & 0x00FF);
		if (reg[x] == k)
			PC = PC + 2;
		break;
	}
	case(0x4000): //0x4xkk
	{
		// Skip next instruction if Vx != kk.
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t k = opcode & 0x00FF;
		if (reg[x] != k)
			PC = PC + 2;
		break;
	}
	case(0x5000): //0x5xy0
	{	// Skip next instruction if Vx = Vy.
		uint8_t x = (opcode & 0x0F00)>>8;
		uint8_t y = (opcode & 0x00F0)>>4;
		if (reg[x] == reg[y])
			PC = PC + 2;
		break;
	}
	case(0x6000): //0x6xkk
	{
		// Set Vx = kk.
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t k = opcode & 0x00FF;
		reg[x] = k;
		break;
	}
	case(0x7000): //0x7xkk
	{
		// Set Vx = Vx + kk.
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t k = opcode & 0x00FF;
		reg[x] = reg[x] + k;
		break;
	}
	case(0x8000):
		switch (opcode & 0x000F)
		{
		case(0x0000): //0x8xy0
		{
			// Set Vx = Vy
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			reg[x] = reg[y];
			// Reset Vf
			reg[0xF] = 0;
			break;
		}
		case(0x0001): //0x8xy1
		{
			// Set Vx = Vx OR Vy
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			reg[x] = reg[x] | reg[y];
			// Reset Vf
			reg[0xF] = 0;
			break;
		}
		case(0x0002): //0x8xy2
		{
			// Set Vx = Vx AND Vy
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			reg[x] = reg[x] & reg[y];
			// Reset Vf
			reg[0xF] = 0;
			break;
		}
		case(0x0003): //0x8xy3
		{
			// Set Vx = Vx XOR Vy
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			reg[x] = reg[x] ^ reg[y];
			// Reset Vf
			reg[0xF] = 0;
			break;
		}
		case(0x0004): //0x8xy4
		{
			// Set Vx = Vx + Vy, set VF = carry.
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			uint16_t sum = reg[x] + reg[y];
			reg[x] = uint8_t(sum&0x00FF);
			//reg[x] = reg[x] + reg[y];
			if (sum > 0xFF)
				reg[0xF] = 1;
			else
				reg[0xF] = 0;
			break;
		}
		case(0x0005): //0x8xy5
		{
			// Set Vx = Vx - Vy, set VF = NOT borrow. // Ambiguity VF = burrow
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			if (reg[x] < reg[y])
				reg[0xF] = 0;
			else
				reg[0xF] = 1;
			reg[x] = reg[x] - reg[y];

			break;
		}
		case(0x0006): //0x8xy6
		{
			// Set Vx = Vx SHR 1 // Ambiguity Vx = Vy SHR 1
			// If the least - significant bit of Vx is 1,
			// then VF is set to 1, otherwise 0.
			uint8_t x = (opcode & 0x0F00) >> 8;
			//uint8_t y = (opcode & 0x00F0) >> 4;
			//reg[x] = reg[y];
			reg[0xF] = reg[x] & 0x01;
			reg[x] = (reg[x] >> 1);
			// reg[y] = (reg[y] >> reg[x]);
			break;
		}
		case(0x0007): //0x8xy7
		{
			// Set Vx = Vy - Vx, set VF = NOT borrow
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t y = (opcode & 0x00F0) >> 4;
			if (reg[x] > reg[y])
				reg[0xF] = 0;
			else
				reg[0xF] = 1;
			reg[x] = reg[y] - reg[x];
			break;
		}
		case(0x000E): //0x8xyE
		{
			// Set Vx = Vx SHL 1.  // Ambiguity Vx = Vy SHL 1
			// If the most-significant bit of Vx is 1
			// then VF is set to 1, otherwise to 0.
			uint8_t x = (opcode & 0x0F00) >> 8;
			//uint8_t y = (opcode & 0x00F0) >> 4;
			//reg[x] = reg[y];
			reg[0xF] = reg[x] >> 7;
			reg[x] = reg[x] << 1;
			break;
		}
		default:
			break;
		}
		break;
	case(0x9000): //0x9xy0
	{
		// Skip next instruction if Vx != Vy.
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t y = (opcode & 0x00F0) >> 4;
		if (reg[x] != reg[y])
			PC = PC + 2;
		break;
	}
	case(0xA000): //0xAnnn
		// Set I = nnn.
		I = opcode & 0x0FFF;
		break;
	case(0xB000): //0xBnnn
		// Jump to location nnn + V0
		PC = (opcode & 0x0FFF) + reg[0];
		break;
	case(0xC000): //0xCxkk
	{
		// Set Vx = random byte (0 to 255) AND kk
		uint8_t x = (opcode & 0x0F00)>>8;
		uint8_t k = (opcode & 0x00FF);
		uint8_t rand_byte = uint8_t(rand() % 256);
		reg[x] = rand_byte & k;
		break;
	}
	case(0xD000): //0xDxyn
	{
		// Display n-byte sprite starting at memory
		// location I at (Vx, Vy), set VF = collision, ie set VF if any pixel was on before the flip(XOR)
		uint8_t n = opcode & 0x000F;
		uint8_t x = (opcode & 0x0F00)>>8;
		uint8_t y = (opcode & 0x00F0)>>4;
		for (uint8_t row = 0; row < n; row++) {
			for (uint8_t col = 0; col < 8; col++) {
				int8_t pos_x = (reg[x] + col) % 64;
				int8_t pos_y = (reg[y] + row) % 32;
				//if ((pos_x >= 0 && pos_x < 64) && (pos_y >= 0 && pos_y < 32)) {
					uint8_t pixel_val = ((memory[I + row] >> (7 - col)) & 0x01);
					if (pixel_val == 0x01) {
						if (graphics[(pos_y * 64 + pos_x)%2048])
							reg[0xF] = 1;
						else
							reg[0xF] = 0;
						graphics[(pos_y * 64 + pos_x)%2048] ^= 1;	// Flip the value of pixel ie 0x00^0x01 = 0x01 and 0x01^0x01 = 0x00 (Note 8 bit is used here to represent a pixel and not a bit)
						
					}
				//}
			}
		}
		break;
	}
	case(0xE000):
		switch (opcode & 0x00FF)
		{
		case(0x009E): // 0xEx9E
		{
			//Skip next instruction if key with the value of Vx is pressed
			std::map<uint8_t,char> key_map;
			key_map[0x1]='1';
			key_map[0x2]='2';
			key_map[0x3]='3';
			key_map[0xC]='4';
			key_map[0x4]='Q';
			key_map[0x5]='W';
			key_map[0x6]='E';
			key_map[0xD]='R';
			key_map[0x7]='A';
			key_map[0x8]='S';
			key_map[0x9]='D';
			key_map[0xE]='F';
			key_map[0xA]='Z';
			key_map[0x0]='X';
			key_map[0xB]='C';
			key_map[0xF]='V';
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t keyCode = key_map[reg[x]];
			// If the most significant bit of return value of GetAsyncKeyState is set
			// the key 'k' is pressed
			if (GetAsyncKeyState(keyCode) & 0x8000) {
				PC += 2;
			}
			break;
		}
		case(0x00A1): // 0xExA1
		{
			//Skip next instruction if key with the value of Vx is not pressed
			std::map<uint8_t, char> key_map;
			key_map[0x1] = '1';
			key_map[0x2] = '2';
			key_map[0x3] = '3';
			key_map[0xC] = '4';
			key_map[0x4] = 'Q';
			key_map[0x5] = 'W';
			key_map[0x6] = 'E';
			key_map[0xD] = 'R';
			key_map[0x7] = 'A';
			key_map[0x8] = 'S';
			key_map[0x9] = 'D';
			key_map[0xE] = 'F';
			key_map[0xA] = 'Z';
			key_map[0x0] = 'X';
			key_map[0xB] = 'C';
			key_map[0xF] = 'V';
			uint8_t x = (opcode & 0x0F00) >> 8;
			uint8_t keyCode = key_map[reg[x]];
			// If the most significant bit of return value of GetAsyncKeyState is set
			// the key 'k' is pressed
			
			if (!(GetAsyncKeyState(keyCode) & 0x8000)) {
				PC += 2;
			}
			break;
		}
		default:
			std::cout << "Unknown Instruction 1: "<<hex<<opcode<<endl;
			break;
		}
		break;
	case(0xF000):
		switch (opcode & 0x00FF)
		{
		case(0x0007): //0xFx07
		{
			// Set Vx = delay timer value.
			uint8_t x = (opcode&0x0F00)>>8;
			reg[x] = delay_timer;
			break;
		}
		case(0x000A): //0xFx0A
		{
			// Wait for a key press,
			// store the value of the key in Vx.
			uint8_t x = (opcode & 0x0F00) >> 8;
			std::map<uint8_t, char> key_map;
			key_map[0x1] = '1';
			key_map[0x2] = '2';
			key_map[0x3] = '3';
			key_map[0xC] = '4';
			key_map[0x4] = 'Q';
			key_map[0x5] = 'W';
			key_map[0x6] = 'E';
			key_map[0xD] = 'R';
			key_map[0x7] = 'A';
			key_map[0x8] = 'S';
			key_map[0x9] = 'D';
			key_map[0xE] = 'F';
			key_map[0xA] = 'Z';
			key_map[0x0] = 'X';
			key_map[0xB] = 'C';
			key_map[0xF] = 'V';
			bool keyPressed = false;
			int keyIndex = 0;
			for(keyIndex = 0; keyIndex < 16; keyIndex++) {
				uint8_t keyCode = key_map[keyIndex];
				// If the most significant bit of return value of GetAsyncKeyState is set
				// the key 'k' is pressed
				if ((GetAsyncKeyState(keyCode) & 0x8000)) {
					PC += 2;
					keyPressed = true;
					break;
				}
			}
			if (keyPressed) {
				reg[x] = keyIndex;
			}
			else
				PC -= 2;

			
			/*
			std::map<char, uint8_t> key_map;
			key_map['1'] = 0x01;
			key_map['2'] = 0x02;
			key_map['3'] = 0x03;
			key_map['4'] = 0x0C;
			key_map['Q'] = 0x04;
			key_map['W'] = 0x05;
			key_map['E'] = 0x06;
			key_map['R'] = 0x0D;
			key_map['A'] = 0x07;
			key_map['S'] = 0x08;
			key_map['D'] = 0x09;
			key_map['F'] = 0x0E;
			key_map['Z'] = 0x0A;
			key_map['X'] = 0x00;
			key_map['C'] = 0x0B;
			key_map['V'] = 0x0F;
			key_map['q'] = 0x04;
			key_map['w'] = 0x05;
			key_map['e'] = 0x06;
			key_map['r'] = 0x0D;
			key_map['a'] = 0x07;
			key_map['s'] = 0x08;
			key_map['d'] = 0x09;
			key_map['f'] = 0x0E;
			key_map['z'] = 0x0A;
			key_map['x'] = 0x00;
			key_map['c'] = 0x0B;
			key_map['v'] = 0x0F;
			//char key = _getch();
			if (!(GetAsyncKeyState(keyCode) & 0x8000)) {
				PC += 2;
			}
			reg[x] = key_map[key];
			*/
			break;
		}
		case(0x0015): //0xFx15
		{
			// Set delay timer = Vx
			uint8_t x = (opcode & 0x0F00) >> 8;
			delay_timer = reg[x];
			break;
		}
		case(0x0018): //0xFx18
		{
			// Set sound timer = Vx.
			uint8_t x = (opcode & 0x0F00) >> 8;
			sound_timer = reg[x];
			//cout << "----------Ball_y: " << int(reg[7]) << endl;
			//cout << "----------Padd_y: " << int(reg[0xB]) << endl;
			break;
		}
		case(0x001E): //0xFx1E
		{
			// Set I = I + Vx.
			uint8_t x = (opcode & 0x0F00) >> 8;
			I = I + reg[x];
			break;
		}
		case(0x0029): //0xFx29
		{
			// Set I = location of sprite for digit Vx.
			uint8_t x = (opcode & 0x0F00) >> 8;
			I = 5*reg[x];
			break;
		}
		case(0x0033): //0xFx33
		{
			// Store BCD representation of Vx in memory locations I, I+1, and I+2.
			// Hundreds digit in memory at location in I, the tens digit at location I + 1, and the ones digit at location I + 2
			uint8_t x = (opcode & 0x0F00) >> 8;
			/*
			uint8_t num1 = reg[x] % 10;
			uint8_t num2 = (reg[x] % 100 - num1)/10;
			uint8_t num3 = (reg[x] % 1000 - num2*10 - num1)/100;
			*/
			int hundreds = reg[x] / 100;
			int tens = (reg[x] / 10) % 10;
			int digits = reg[x] % 10;

			memory[I] = hundreds;
			memory[I+1] = tens;
			memory[I+2] = digits;
			break;
		}
		case(0x0055): //0xFx55
		{
			// Store registers V0 through Vx in memory starting at location I.
			uint8_t x = (opcode & 0x0F00) >> 8;
			for (uint8_t i = 0; i <= x; i++) {
				memory[I + i] = reg[i];
			}
			break;
		}
		case(0x0065): //0xFx65
		{
			// Read registers V0 through Vx from memory starting at location I.
			// I is then set to I+x+1
			uint8_t x = (opcode & 0x0F00) >> 8;
			for (uint8_t i = 0; i <= x; i++) {
				reg[i] = memory[I + i];
			}
			I = I + x + 1;
			break;
		}
		default:
			std::cout << "Unknown Instruction: "<<hex<<opcode<<endl;
			break;
		}
		break;
	default:
		std::cout << "Op-code not found\n";
		break;
	}
	
	// 100 Hz Frequecy
	Sleep(10);	// Sleep for 17ms F = 1000/10Hz = 100Hz 
	// Update timers
	if (delay_timer > 0) {
		delay_timer--;
	}
	if (sound_timer > 0) {
		if (sound_timer == 1) {
			std::cout << "Beep beep\n";
		}
		sound_timer--;
	}
}