#ifndef CPU_H
#define CPU_H

#include "utils/util.h"

typedef struct
{
    u8 A;   // 8-bit Accumlator
    u16 PC; // 16 bit Program Counter
    u8 SP;  // Stack Pointer
    u8 X;   // 'X' Index Register
    u8 Y;   // 'Y' Index Register
    u8 memory[MEMORY_MAP_SIZE]; // System Memory
    bool N; // Negative Flag
    bool V; // Overflow Flag
    bool B; // B Flag
    bool D; // Decimal Flag
    bool I; // Interrupt Disable
    bool Z; // Zero Flag
    bool C; // Carry Flag
    bool running;
    u64 global_cycles;
} cpu_t;

void cpu_init(cpu_t *cpu);
void cpu_cycle(cpu_t *cpu);
u8 load_program(cpu_t *cpu, const char* rom_path, u16 load_addr);
u8 read_memory(cpu_t *cpu, u16 address);
void write_memory(cpu_t *cpu, u16 address, u8 value);
void cpu_display_registers(cpu_t *cpu);
void print_memory(cpu_t *cpu, u16 start, u16 end);

// Flags
void N_flag(cpu_t *cpu, u8 value);
void V_flag(cpu_t *cpu, u8 value, u8 result);
void B_flag(cpu_t *cpu, u8 value);
void D_flag(cpu_t *cpu, u8 value);
void I_flag(cpu_t *cpu, u8 value);
void Z_flag(cpu_t *cpu, u8 value);
void C_flag(cpu_t *cpu, u8 value);

#endif