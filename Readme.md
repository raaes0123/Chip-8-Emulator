# Chip-8 Emulator
CHIP-8 is an interpreted programming language, developed on an 1802 microprocessor. CHIP-8 was designed to be easy to program for. It was intended as a simpler way to make small programs and games for the computer.
## Machine Description
### Memory
Chip-8 has 4096 (0x1000) memory locations, all of which are 8 bits (a byte) which is where the term CHIP-8 originated. However, the CHIP-8 interpreter itself occupies the first 512 bytes of the memory space on these machines. For this reason, most programs written for the original system begin at memory location 512 (0x200) and do not access any of the memory below the location 512 (0x200). The uppermost 256 bytes (0xF00-0xFFF) are reserved for display refresh, and the 96 bytes below that (0xEA0-0xEFF) were reserved for the call stack, internal use, and other variables.
### Registers
CHIP-8 has 16 8-bit data registers named V0 to VF. The VF register doubles as a flag for some instructions; thus, it should be avoided. In an addition operation, VF is the carry flag, while in subtraction, it is the "no borrow" flag. In the draw instruction VF is set upon pixel collision.

The address register, which is named I, is 12 bits wide and is used with several opcodes that involve memory operations. Program Counter (PC) holds the address of next instruction to be executed.
### The stack
The stack is only used to store return addresses when subroutines are called. The original RCA 1802 version allocated 48 bytes for up to 12 levels of nesting.
### Timers
CHIP-8 has two timers. They both count down at 60 hertz, until they reach 0.
- Delay timer: This timer is intended to be used for timing the events of games. Its value can be set and read.
- Sound timer: This timer is used for sound effects. When its value is nonzero, a beeping sound is made. Its value can only be set.
### Input
Input is done with a hex keyboard that has 16 keys ranging 0 to F. The '8', '4', '6', and '2' keys are typically used for directional input. Three opcodes are used to detect input. One skips an instruction if a specific key is pressed, while another does the same if a specific key is not pressed. The third waits for a key press, and then stores it in one of the data registers.
### Graphics and sound
Original CHIP-8 display resolution is 64×32 pixels, and color is monochrome. Graphics are drawn to the screen solely by drawing sprites, which are 8 pixels wide and may be from 1 to 15 pixels in height. Sprite pixels are XOR'd with corresponding screen pixels. In other words, sprite pixels that are set flip the color of the corresponding screen pixel, while unset sprite pixels do nothing. The carry flag (VF) is set to 1 if any screen pixels are flipped from set to unset when a sprite is drawn and set to 0 otherwise. This is used for collision detection.

As previously described, a beeping sound is played when the value of the sound timer is nonzero.
### Opcode
CHIP-8 has 35 opcodes, which are all two bytes long and stored big-endian.
## Building
Raylib is used to render the graphics. So install Raylib before running make. Make sure that RAYLIB_PATH and COMPILER_PATH points to right location in your system.
```
$ make
```
## Running
Make builds chip_8 executable which you can run.
```
./chip_8
```
## Demo
![Chip-8 Demo gif](https://github.com/raaes0123/Chip-8-Emulator/blob/main/asset/Missile.gif)