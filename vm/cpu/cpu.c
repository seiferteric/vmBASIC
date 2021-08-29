#include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t  gp_registers[N_REGS];
static uint16_t IP;
static uint8_t  IR;
static uint8_t  SR;

static int  reset_rom(char *);
static void reset_ram(void);
static void reset_registers(void);
static int  execute_instruction(void);
static int  reset(char *);
static void reset_registers(void);
static void print_regs();
static int  tick();

static union {
    uint8_t mem[RAM_SIZE + ROM_SIZE];
    struct {
        uint8_t ram[RAM_SIZE];
        uint8_t rom[ROM_SIZE];
    }ram_rom;
}mem_map;

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
  IP = RAM_SIZE;
  IR = 0;
  SR = 0;
  memset(gp_registers, 0, sizeof(gp_registers));
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
  IR = mem_map.mem[IP];
  
  union Args *args = (union Args *)&mem_map.mem[IP + 1];
  
  switch (IR) {
  case 0x00: // halt
    return 0;
  case 0x01: // mov reg, reg
    gp_registers[args->reg_reg.reg1] = gp_registers[args->reg_reg.reg2];
    break;
  case 0x02: // mov reg, imm
    gp_registers[args->reg_imm.reg] = args->reg_imm.num;
    break;
  case 0x03: // add reg, reg
    gp_registers[args->reg_reg.reg1] += gp_registers[args->reg_reg.reg2];
    break;
  case 0x04: // add reg, imm
    gp_registers[args->reg_imm.reg] += args->reg_imm.num;
    break;

  default:
    printf("Invalid Instruction: 0x%02x at 0x%02x\n", (unsigned int)IR,
           (unsigned int)IP);
    break;
  }
  //Reached end of ROM, lets rest
  if(IP == 0xFFFF) {
      reset_ram();
      reset_registers();
      return 1;
  }
  IP += INST_SZ;
  return 1;
}
void print_regs() {
  int i = 0;
  printf("$%02x: ", (unsigned int)IP);
  for (; i < N_REGS; i++) {
    printf("%u ", (unsigned int)gp_registers[i]);
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
