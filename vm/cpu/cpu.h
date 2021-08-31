#pragma once
//Mem map stuff
//Need to reduce to accomodate video, audio, hardware etc.
#define ROM_SIZE 0x8000
#define RAM_SIZE 0x8000

#define RAM_START 0
#define RAM_END   0x7FFF
#define ROM_START 0x8000
#define ROM_END   0xFFFF



//Register stuff
#define N8_REGS  32
#define INST_SZ  5

void run(char*);

// Mem Map:
//
// 0x0000 - 0x7FFF -> RAM
// 0x8000 - 0xFFFF -> ROM
