#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "utils/util.h"
#include "cpu.h"

enum ADDR_MODES
{
    IMM,
    ZP,
    ZPX,
    ZPY,
    ABS,
    ABX,
    ABY,
    IND,
    IDX,
    IDY,
    IMP,
    REL
};

typedef struct opcode_t
{
    u8 addr_mode;                             // Addressing Modes;   ;
    u8 cycles;                                // Base Cycle Count
    void (*operation)(cpu_t *cpu, u16 addr); // Pointer to Function Implementation
} opcode_t;

extern opcode_t opcodes[256];

u8 imm_address(cpu_t *cpu);
u16 zp_address(cpu_t *cpu);
u16 zpx_address(cpu_t *cpu);
u16 zpy_address(cpu_t *cpu);
u16 abs_address(cpu_t *cpu);
u16 abx_address(cpu_t *cpu);
u16 aby_address(cpu_t *cpu);
u16 ind_address(cpu_t *cpu);
u16 indx_address(cpu_t *cpu);
u16 indy_address(cpu_t *cpu);
u16 imp_address(cpu_t *cpu);
i8 rel_address(cpu_t *cpu);

// Load/Store
void LDA(cpu_t *cpu, u16 addr);
void LDX(cpu_t *cpu, u16 addr);
void LDY(cpu_t *cpu, u16 addr);
void STA(cpu_t *cpu, u16 addr);
void STX(cpu_t *cpu, u16 addr);
void STY(cpu_t *cpu, u16 addr);

// Increment/Decrement
void INC(cpu_t *cpu, u16 addr);
void INX(cpu_t *cpu, u16 addr);
void INY(cpu_t *cpu, u16 addr);
void DEC(cpu_t *cpu, u16 addr);
void DEX(cpu_t *cpu, u16 addr);
void DEY(cpu_t *cpu, u16 addr);

// Stack
void PHA(cpu_t *cpu, u16 addr); // Push Accumulator
void PHP(cpu_t *cpu, u16 addr); // Push Processor Status
void PLA(cpu_t *cpu, u16 addr); // Pull Accumulator
void PLP(cpu_t *cpu, u16 addr); // Pull Processor

// Branch
void BCC(cpu_t *cpu, u16 addr); // Branch on Carry Clear
void BCS(cpu_t *cpu, u16 addr); // Branch on Carry Set
void BEQ(cpu_t *cpu, u16 addr); // Branch on Equal
void BNE(cpu_t *cpu, u16 addr); // Branch on Not Equal
void BMI(cpu_t *cpu, u16 addr); // Branch on Minus
void BPL(cpu_t *cpu, u16 addr); // Branch on Plus
void BVC(cpu_t *cpu, u16 addr); // Branch on Overflow Clear
void BVS(cpu_t *cpu, u16 addr); // Branch on Overflow Set

// Jump
void JMP(cpu_t *cpu, u16 addr); // Jump
void JSR(cpu_t *cpu, u16 addr); // Jump to Subroutine
void RTS(cpu_t *cpu, u16 addr); // Return from Subroutine
void RTI(cpu_t *cpu, u16 addr); // Return from Interrupt

// Transfer
void TAX(cpu_t *cpu, u16 addr); // Transfer Accumulator to X
void TAY(cpu_t *cpu, u16 addr); // Transfer Accumulator to Y
void TXA(cpu_t *cpu, u16 addr); // Transfer X to Accumulator
void TYA(cpu_t *cpu, u16 addr); // Transfer Y to Accumulator
void TSX(cpu_t *cpu, u16 addr); // Transfer Stack Pointer to X
void TXS(cpu_t *cpu, u16 addr); // Transfer X to Stack Pointer

// Arithmetic & Logic
void ADC(cpu_t *cpu, u16 addr);
void SBC(cpu_t *cpu, u16 addr);
void AND(cpu_t *cpu, u16 addr);
void EOR(cpu_t *cpu, u16 addr);
void ORA(cpu_t *cpu, u16 addr);
void CMP(cpu_t *cpu, u16 addr);
void CPX(cpu_t *cpu, u16 addr);
void CPY(cpu_t *cpu, u16 addr);

// Shift and Rotate Operations
void ASL(cpu_t *cpu, u16 addr);
void LSR(cpu_t *cpu, u16 addr);
void ROL(cpu_t *cpu, u16 addr);
void ROR(cpu_t *cpu, u16 addr);

void SEC(cpu_t *cpu, u16 addr); // Set Carry Flag
void SED(cpu_t *cpu, u16 addr); // Set Decimal Flag
void SEI(cpu_t *cpu, u16 addr); // Set Interrupt Disable
void CLC(cpu_t *cpu, u16 addr); // Clear Carry Flag
void CLD(cpu_t *cpu, u16 addr); // Clear Decimal Flag
void CLI(cpu_t *cpu, u16 addr); // Clear Interrupt Disable
void CLV(cpu_t *cpu, u16 addr); // Clear Overflow Flag

void BIT(cpu_t *cpu, u16 addr);
void BRK(cpu_t *cpu, u16 addr);
void NOP(cpu_t *cpu, u16 addr);

#endif