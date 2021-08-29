#pragma once
#define ROM_SIZE 0x8000
#define RAM_SIZE 0x8000
#define N_REGS 4
#define INST_SZ 5
  
void run(char*);

// Mem Map:
//
// 0x0000 - 0x7FFF -> RAM
// 0x8000 - 0xFFFF -> ROM
