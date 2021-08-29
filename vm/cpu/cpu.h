#pragma once
#include <stdint.h>
#define ROM_SIZE 0x8000
#define RAM_SIZE 0x8000
#define N_REGS 4
#define INST_SZ 5

struct Reg_Reg {
    uint8_t reg1;
    uint8_t reg2;
};
struct Reg_Addr {
    uint8_t reg;
    uint16_t addr;
};
struct Addr_Reg {
    uint16_t addr;
    uint8_t reg;
};
struct Reg_Imm {
    uint8_t reg;
    uint8_t num;
};

union Args {
    struct Reg_Reg reg_reg;
    struct Addr_Reg addr_reg;
    struct Reg_Addr reg_addr;
    struct Reg_Imm reg_imm;
};

  
void run(char*);

// Mem Map:
//
// 0x0000 - 0x7FFF -> RAM
// 0x8000 - 0xFFFF -> ROM
