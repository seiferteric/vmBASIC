#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct {
    uint16_t IP;                   //Instruction Pointer
    uint8_t  SR;                   //Status Register
    uint16_t SP;                   //Stack Pointer
    uint8_t  gp8_regs[N8_REGS];    //General Purpose 8 bit Registers
}reg;


static int  reset_rom(char *);
static void reset_ram(void);
static void reset_registers(void);
static int  execute_instruction(void);
static int  reset(char *);
static void reset_registers(void);
static void print_regs();
static int  tick();

#pragma pack(1)
struct Instruction {
    uint8_t op;             //Instruction OP Code
    union {
        uint8_t data[4];    //Access arguments as raw bytes (dor debugging mainly)
        struct {
            uint8_t reg1;
            uint8_t reg2;
        }reg_reg;
        struct {
            uint8_t reg;
            uint16_t addr;
        }reg_addr;
        struct {
            uint16_t addr;
            uint8_t reg;
        }addr_reg;
        struct {
            uint8_t reg;
            uint8_t num;
        }reg_imm;
    }args;
};

static union {
    uint8_t mem[RAM_SIZE + ROM_SIZE];
    struct {
        uint8_t ram[RAM_SIZE];
        uint8_t rom[ROM_SIZE];
    }ram_rom;
}mem_map;

#pragma pack()

int reset(char *rom_file) {
  reset_registers();
  if (reset_rom(rom_file) != 0) {
    printf("Invalid ROM\n");
    return -1;
  }
  reset_ram();
  return 0;
}

void reset_registers(void) {

  memset(&reg, 0, sizeof(reg));
  //Set Instruction Pointer to top of ROM
  reg.IP = ROM_START;
}
void reset_ram(void) { memset(mem_map.ram_rom.ram, 0, RAM_SIZE); }
int reset_rom(char *rom_file) {
  FILE *f = fopen(rom_file, "rb");
  if (f) {
    size_t b_read = fread(mem_map.ram_rom.rom, ROM_SIZE, 1, f);
    if (b_read > ROM_SIZE)
      return 1;
  } else {
    memset(mem_map.ram_rom.rom, 0, ROM_SIZE);
    return 1;
  }
  return 0;
}
int execute_instruction() {
  
  struct Instruction *inst = (struct Instruction *)&mem_map.mem[reg.IP];
  //printf("%u %u %u %u %u\n", inst->inst, inst->args.data[0], inst->args.data[1], inst->args.data[2], inst->args.data[3]);
  switch (inst->op) {
  case 0x00: // halt
    return 0;
  case 0x01: // mov reg, reg
    reg.gp8_regs[inst->args.reg_reg.reg1] = reg.gp8_regs[inst->args.reg_reg.reg2];
    break;
  case 0x02: // mov reg, imm
    reg.gp8_regs[inst->args.reg_imm.reg] = inst->args.reg_imm.num;
    break;
  case 0x03: // add reg, reg
    reg.gp8_regs[inst->args.reg_reg.reg1] += reg.gp8_regs[inst->args.reg_reg.reg2];
    break;
  case 0x04: // add reg, imm
    reg.gp8_regs[inst->args.reg_imm.reg] += inst->args.reg_imm.num;
    break;

  default:
    printf("Invalid Instruction: 0x%02x at 0x%02x\n", (unsigned int)inst->op,
           (unsigned int)reg.IP);
    break;
  }
  //Reached end of ROM, lets rest
  if(reg.IP == 0xFFFF) {
      reset_ram();
      reset_registers();
      return 1;
  }
  reg.IP += INST_SZ;
  return 1;
}
void print_regs() {
  int i = 0;
  printf("$%02x: ", (unsigned int)reg.IP);
  for (; i < N8_REGS; i++) {
    printf("%u ", (unsigned int)reg.gp8_regs[i]);
  }
  printf("\n");
}
int tick() { return execute_instruction(); }

void run(char *rom_file) {
  if (reset(rom_file) == -1)
    return;
  print_regs();
  while (tick() != 0) {
    print_regs();
  }
}
