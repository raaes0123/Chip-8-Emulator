// Chip_8_Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "./include/emulator.h"
//#include <windows.h>
#include <conio.h>
//#include "raylib-cpp.hpp"  //for C++ API
#include "raylib.h"
#include <fstream>

using namespace std;

void fileRead(Emulator &emu,int game_choice){
	// ROM
	// Filestream Object with binary as argument
	// ifstream filestream("test_opcode.ch8", ios::binary);
	// ifstream filestream("flightrunner.ch8", ios::binary);
	//ifstream filestream("./roms/PONG", ios::binary);
	// ifstream filestream("./roms/MISSILE", ios::binary);
	// ifstream filestream("KeypadTest.ch8", ios::binary);
	// ifstream filestream("ultimatetictactoe.ch8", ios::binary);
	// ifstream filestream("PONG2", ios::binary);
	// ifstream filestream("br8kout.ch8", ios::binary);
	//ifstream filestream("test_2.ch8", ios::binary);
	
	ifstream filestream;
	string game[] = {"./roms/PONG","./roms/MISSILE"};
	filestream.open(game[game_choice], ios::binary);

	// Check if file can be opened and if yes
	if (filestream.is_open()) {
		// Get the size of the file
		// Set position at the end of the file
		filestream.seekg(0, filestream.end);
		// Get current position (end of the file)
		int fileSize = filestream.tellg();
		// Set position at the beginning of the file again
		filestream.seekg(0, filestream.beg);
		// Copy the contents of the file to a buffer
		char* buf = new char[fileSize];
		filestream.read(buf, fileSize);
		// Close the file
		filestream.close();

		// Read from buffer into the memory
		for (int i = 0; i < fileSize; i++) {
			emu.memory[0x200 + i] = buf[i];
		}
	}
}

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int pixelWidth = 20;
	const int pixelHeight = 20;
	const int actualWidth = 64;
	const int actualHeight = 32;
	const int screenWidth = actualWidth * pixelWidth;
	const int screenHeight = actualHeight * pixelHeight;
	const Color color = DARKGREEN;
	InitWindow(screenWidth, screenHeight, "Chip-8_Emulator");
	
	bool game_state = 0;
	int game_choice = 0;
	SetTargetFPS(600);               // Set our game to run at 600 frames-per-second
	//--------------------------------------------------------------------------------------
	
	Emulator emu;
	emu.initialize();

	//--------------------------------------------------------------------------------------
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if(game_state == 0){
			// Splash Screen
			// Display Screen
			BeginDrawing();
			ClearBackground(BLACK);
			const char * message_1 = "WELCOME";
			DrawText(message_1, 530, 200, 40, color);
			const char * message_2 = "TO";
			DrawText(message_2, 600, 250, 40, color);
			const char * message_3 = "Chip-8 Emulator";
			DrawText(message_3, 400, 300, 60, color);
			const char * message_4 = "Press Space to continue";
			DrawText(message_4, 450, 500, 30, color);
			const char * message_5 = "PONG";
			DrawText(message_5, 600, 400, 20, color);
			const char * message_6 = "MISSILE";
			DrawText(message_6, 590, 430, 20, color);

			// Choose between the two games: PONG and MISSILE
			if(IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_DOWN))
				game_choice = (game_choice + 1) % 2;
			if(game_choice == 0){
				DrawLine(595,410,660,410,color);
			}
			else{
				DrawLine(585,440,680,440,color);
			}
			// Wait for SPACE and continue
			if(IsKeyPressed(KEY_SPACE)){
				fileRead(emu,game_choice);
				game_state = 1;
			}
			EndDrawing();
		}
		else{
			bool drawScreen = false;
			BeginDrawing();

			uint8_t opcode = (emu.memory[emu.PC] & 0xF0);
			if (opcode == 0xD0)
				drawScreen = true;
			// Call emulateCycle funtion
			emu.emulateCycle();
			// Draw pixel from the graphic memory only when Dxyn instruction is called
			if (drawScreen)
			{
				ClearBackground(BLACK);
				for (int posX = 0; posX < actualWidth; posX++)
				{
					for (int posY = 0; posY < actualHeight; posY++)
					{
						if (emu.graphics[posX + 64 * posY])
						{
							DrawRectangle(posX * pixelWidth, posY * pixelHeight, pixelWidth, pixelHeight, color);
						}
					}
				}
				drawScreen = false;
			}
			EndDrawing();
		}
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
