#include "instruction.h"

u16 imm_address(cpu_t *cpu) {
    return cpu->PC++;
}

u16 zp_address(cpu_t *cpu) {
    u8 addr = read_memory(cpu, cpu->PC++);
    return addr;
}

u16 zpx_address(cpu_t *cpu) {
    u8 addr = read_memory(cpu, cpu->PC++);
    return (addr + cpu->X) & 0xFF;
}

u16 zpy_address(cpu_t *cpu) {
    u8 addr = read_memory(cpu, cpu->PC++);
    return (addr + cpu->Y) & 0xFF;
}

u16 abs_address(cpu_t *cpu) {
    u16 lo = read_memory(cpu, cpu->PC++);
    u16 hi = read_memory(cpu, cpu->PC++);
    return (hi << 8) | lo;  
}

u16 abx_address(cpu_t *cpu) {
    u16 lo = read_memory(cpu, cpu->PC++);
    u16 hi = read_memory(cpu, cpu->PC++);
    u16 base = (hi << 8) | lo;
    return base + cpu->X; 
}

u16 aby_address(cpu_t *cpu) {
    u16 lo = read_memory(cpu, cpu->PC++);
    u16 hi = read_memory(cpu, cpu->PC++);
    u16 base = (hi << 8) | lo;
    return base + cpu->Y; 
}

u16 ind_address(cpu_t *cpu) {
    u16 ptr_lo = read_memory(cpu, cpu->PC++);
    u16 ptr_hi = read_memory(cpu, cpu->PC++);
    u16 ptr = (ptr_hi << 8) | ptr_lo;

    // Emulate 6502 page-boundary bug
    u16 lo = read_memory(cpu, ptr);
    u16 hi;
    if ((ptr & 0x00FF) == 0x00FF) {
        hi = read_memory(cpu, ptr & 0xFF00); // wraps around page
    } else {
        hi = read_memory(cpu, ptr + 1);
    }
    return (hi << 8) | lo;
}

u16 indx_address(cpu_t *cpu) {
    u8 zp_addr = (read_memory(cpu, cpu->PC++) + cpu->X) & 0xFF;
    u8 lo = read_memory(cpu, zp_addr);
    u8 hi = read_memory(cpu, (zp_addr + 1) & 0xFF);
    return (hi << 8) | lo;
}

u16 indy_address(cpu_t *cpu) {
    u8 zp_addr = read_memory(cpu, cpu->PC++);
    u8 lo = read_memory(cpu, zp_addr);
    u8 hi = read_memory(cpu, (zp_addr + 1) & 0xFF);
    u16 addr = (hi << 8) | lo;
    return addr + cpu->Y;
}

u16 imp_address(cpu_t *cpu) {
    return 0;
}

i8 rel_address(cpu_t *cpu) {
    return (i8)read_memory(cpu, cpu->PC++);
}

void LDA(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    cpu->A = value;

    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void LDX(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    cpu->X = value;

    Z_flag(cpu, cpu->X);
    N_flag(cpu, cpu->X);
}

void LDY(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    cpu->Y = value;

    Z_flag(cpu, cpu->Y);
    N_flag(cpu, cpu->Y);
}

void STA(cpu_t *cpu, u16 addr)
{
    write_memory(cpu, addr, cpu->A);
}

void STX(cpu_t *cpu, u16 addr)
{
    write_memory(cpu, addr, cpu->X);
}

void STY(cpu_t *cpu, u16 addr)
{
    write_memory(cpu, addr, cpu->Y);
}

void INC(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    value = (value + 1) & 0xFF;
    write_memory(cpu, addr, value);

    Z_flag(cpu, value);
    N_flag(cpu, value);
}

void INX(cpu_t *cpu, u16 addr)
{
    cpu->X++;
    Z_flag(cpu, cpu->X);
    N_flag(cpu, cpu->X);
}

void INY(cpu_t *cpu, u16 addr)
{
    cpu->Y++;
    Z_flag(cpu, cpu->Y);
    N_flag(cpu, cpu->Y);
}

void DEC(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    value = (value - 1) & 0xFF;
    write_memory(cpu, addr, value);

    Z_flag(cpu, value);
    N_flag(cpu, value);
}

void DEX(cpu_t *cpu, u16 addr)
{
    cpu->X--;

    Z_flag(cpu, cpu->X);
    N_flag(cpu, cpu->X);
}

void DEY(cpu_t *cpu, u16 addr)
{
    cpu->Y--;

    Z_flag(cpu, cpu->Y);
    N_flag(cpu, cpu->Y);
}

void PHA(cpu_t *cpu, u16 addr)
{
    write_memory(cpu, (0x100 | cpu->SP), cpu->A);
    cpu->SP--;
} 

void PHP(cpu_t *cpu, u16 addr)
{
    u8 value = 0;
    value |= cpu->C ? CARRY_FLAG : 0;  
    value |= cpu->Z ? ZERO_FLAG : 0;  
    value |= cpu->I ? INTERRUPT_FLAG : 0;  
    value |= cpu->D ? DECIMAL_FLAG : 0;  
    value |= BREAK_FLAG;               
    value |= 0x20;               
    value |= cpu->V ? OVERFLOW_FLAG : 0;  
    value |= cpu->N ? NEGATIVE_FLAG : 0;  

    write_memory(cpu, (0x100 | cpu->SP), value);
    cpu->SP--;
}

void PLA(cpu_t *cpu, u16 addr)
{
    cpu->SP++;
    cpu->A = read_memory(cpu, (0x0100 | cpu->SP));
    
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void PLP(cpu_t *cpu, u16 addr)
{
    cpu->SP++;
    u8 value = read_memory(cpu, (0x100 | cpu->SP));

    cpu->C = (value & CARRY_FLAG) ? 1 : 0;
    cpu->Z = (value & ZERO_FLAG) ? 1 : 0;
    cpu->I = (value & INTERRUPT_FLAG) ? 1 : 0;
    cpu->D = (value & DECIMAL_FLAG) ? 1 : 0;
    cpu->B = (value & BREAK_FLAG) ? 1 : 0;
    cpu->V = (value & OVERFLOW_FLAG) ? 1 : 0;
    cpu->N = (value & NEGATIVE_FLAG) ? 1 : 0;
}

void BCC(cpu_t *cpu, u16 addr)
{
    if (!cpu->C) cpu->PC += (i8)addr;
}

void BCS(cpu_t *cpu, u16 addr)
{
    if (cpu->C) cpu->PC += (i8)addr;
}

void BEQ(cpu_t *cpu, u16 addr)
{
    if (cpu->Z) cpu->PC += (i8)addr;
}

void BNE(cpu_t *cpu, u16 addr)
{
    if (!cpu->Z) cpu->PC += (i8)addr;
}

void BMI(cpu_t *cpu, u16 addr)
{
    if (cpu->N) cpu->PC += (i8)addr;
}

void BPL(cpu_t *cpu, u16 addr)
{
    if (!cpu->N) cpu->PC += (i8)addr;
}

void BVC(cpu_t *cpu, u16 addr)
{
    if (!cpu->V) cpu->PC += (i8)addr;
}

void BVS(cpu_t *cpu, u16 addr)
{
    if (cpu->V) cpu->PC += (i8)addr;
}

// Jump
void JMP(cpu_t *cpu, u16 addr)
{
    cpu->PC = addr;
}

void JSR(cpu_t *cpu, u16 addr)
{
    u16 return_addr = cpu->PC - 1;

    // High Byte
    write_memory(cpu, (0x100 | cpu->SP), (return_addr >> 8) & 0xFF);
    cpu->SP--;

    // Low Byte
    write_memory(cpu, (0x100 | cpu->SP), return_addr & 0xFF);
    cpu->SP--;

    cpu->PC = addr;
}

void RTS(cpu_t *cpu, u16 addr)
{
    cpu->SP++;
    u8 lo = read_memory(cpu, (0x100 | cpu->SP));

    cpu->SP++;
    u8 hi = read_memory(cpu, (0x100 | cpu->SP));

    cpu->PC = ((hi << 8) | lo) + 1;
}

void RTI(cpu_t *cpu, u16 addr)
{
    // Resotre Processor Status
    cpu->SP++;
    u8 value = read_memory(cpu, (0x100 | cpu->SP));

    cpu->C = (value & CARRY_FLAG) ? 1 : 0;
    cpu->Z = (value & ZERO_FLAG) ? 1 : 0;
    cpu->I = (value & INTERRUPT_FLAG) ? 1 : 0;
    cpu->D = (value & DECIMAL_FLAG) ? 1 : 0;
    cpu->B = (value & BREAK_FLAG) ? 1 : 0;
    cpu->V = (value & OVERFLOW_FLAG) ? 1 : 0;
    cpu->N = (value & NEGATIVE_FLAG) ? 1 : 0;

    // Low Byte of Return Address
    cpu->SP++;
    u8 lo = read_memory(cpu, (0x100 | cpu->SP));

    // High Byte
    cpu->SP++;
    u8 hi = read_memory(cpu, (0x100 | cpu->SP));

    cpu->PC = (hi << 8) | lo;
}

void TAX(cpu_t *cpu, u16 addr)
{
    cpu->X = cpu->A;

    Z_flag(cpu, cpu->X);
    N_flag(cpu, cpu->X);
}

void TAY(cpu_t *cpu, u16 addr)
{
    cpu->Y = cpu->A;
    
    Z_flag(cpu, cpu->Y);
    N_flag(cpu, cpu->Y);
}

void TXA(cpu_t *cpu, u16 addr)
{
    cpu->A = cpu->X;
    
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
} 

void TYA(cpu_t *cpu, u16 addr)
{
    cpu->A = cpu->Y;
    
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void TSX(cpu_t *cpu, u16 addr)
{
    cpu->X = cpu->SP;
}

void TXS(cpu_t *cpu, u16 addr)
{
    cpu->SP = cpu->X;
}

// Arithmetic & Logic
void ADC(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    u8 carry_in = cpu->C;
    u16 result = cpu->A + value + carry_in;

    if (cpu->D) {
        u16 tmp = (cpu->A & 0x0F) + (value & 0x0F) + carry_in;

        if (tmp > 9) {
            result += 6;
        }
        if (result > 0x99) {
            result += 0x60;
        }
    }

    C_flag(cpu, result);
    V_flag(cpu, value, result);

    cpu->A = (u8)result;

    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void SBC(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    value ^= 0xFF;
    u8 carry_in = cpu->C;
    u16 result = cpu->A + value + carry_in;

    C_flag(cpu, result);
    V_flag(cpu, value, result);

    if (cpu->D) {
        // Decimal mode adjustment
        u16 tmp = result;

        // Low nibble adjust
        if (((cpu->A & 0x0F) - (1 - carry_in)) < (value & 0x0F)) {
            tmp -= 0x06;
        }

        // High nibble adjust
        if (tmp > 0x99) {
            tmp -= 0x60;
        }

        result = tmp;
    }

    cpu->A = (u8)result;

    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void AND(cpu_t *cpu, u16 addr)
{
    cpu->A &= read_memory(cpu, addr);
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void EOR(cpu_t *cpu, u16 addr)
{
    cpu->A ^= read_memory(cpu, addr);
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void ORA(cpu_t *cpu, u16 addr)
{
    cpu->A |= read_memory(cpu, addr);
    Z_flag(cpu, cpu->A);
    N_flag(cpu, cpu->A);
}

void CMP(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    u16 result = (u16)cpu->A - (u16)value;
    cpu->C = (cpu->A >= value);
    cpu->Z = (cpu->A == value);
    cpu->N = ((result & NEGATIVE_FLAG) != 0) ? 1 : 0;
}

void CPX(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    u16 result = (u16)cpu->X - (u16)value;
    cpu->C = (cpu->X >= value);
    cpu->Z = (cpu->X == value);
    cpu->N = ((result & NEGATIVE_FLAG) != 0) ? 1 : 0;
}

void CPY(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    u16 result = (u16)cpu->Y - (u16)value;
    cpu->C = (cpu->Y >= value);
    cpu->Z = (cpu->Y == value);
    cpu->N = ((result & NEGATIVE_FLAG) != 0) ? 1 : 0;
}

void ASL(cpu_t *cpu, u16 addr)
{
    if (addr == 0) { // IMP/accumulator mode
        cpu->C = (cpu->A & 0x80) != 0;
        cpu->A <<= 1;
        Z_flag(cpu, cpu->A);
        N_flag(cpu, cpu->A);
    } else {
        u8 value = read_memory(cpu, addr);
        cpu->C = (value & 0x80) != 0;
        value <<= 1;
        write_memory(cpu, addr, value);
        Z_flag(cpu, value);
        N_flag(cpu, value);
    }
}

void LSR(cpu_t *cpu, u16 addr)
{
    if (addr == 0) { // accumulator mode
        cpu->C = (cpu->A & 0x01) ? 1 : 0;
        cpu->A >>= 1;
        Z_flag(cpu, cpu->A);
        N_flag(cpu, cpu->A); 
    } else {
        u8 value = read_memory(cpu, addr);
        cpu->C = (value & 0x01) ? 1 : 0;
        value >>= 1;
        Z_flag(cpu, value);
        N_flag(cpu, value);
        write_memory(cpu, addr, value);
    }
}

void ROL(cpu_t *cpu, u16 addr)
{
    u8 old_c = cpu->C;

    if (addr == 0) {
        cpu->C = (cpu->A & 0x80) ? 1 : 0;
        cpu->A = (cpu->A << 1) | old_c;
        Z_flag(cpu, cpu->A);                  
        N_flag(cpu, cpu->A); 
    } else {
        u8 value = read_memory(cpu, addr);
        cpu->C = (value & 0x80) != 0;       // bit 7 -> Carry
        value = (value << 1) | old_c;       // shift left, insert old carry

        Z_flag(cpu, value);                  // update Zero flag
        N_flag(cpu, value);                  // update Negative flag

        write_memory(cpu, addr, value);
    }
}

void ROR(cpu_t *cpu, u16 addr)
{
    u8 old_c = cpu->C;

    if (addr == 0) {
        cpu->C = (cpu->A & CARRY_FLAG) != 0;       // bit 0 -> Carry
        cpu->A = (cpu->A >> 1) | (old_c << 7);     // shift right, insert old carry

        Z_flag(cpu, cpu->A);                       // update Zero flag
        N_flag(cpu, cpu->A);                       // update Negative flag
    } else {
        u8 value = read_memory(cpu, addr);
        cpu->C = (value & CARRY_FLAG) != 0;       // bit 0 -> Carry
        value = (value >> 1) | (old_c << 7);// shift right, insert old carry

        Z_flag(cpu, value);                  // update Zero flag
        N_flag(cpu, value);                  // update Negative flag

        write_memory(cpu, addr, value);
    }
}

void SEC(cpu_t *cpu, u16 addr)
{
    cpu->C = true;
}

void SED(cpu_t *cpu, u16 addr)
{
    cpu->D = true;
}

void SEI(cpu_t *cpu, u16 addr)
{
    cpu->I = true;
}

void CLC(cpu_t *cpu, u16 addr)
{
    cpu->C = false;
}

void CLD(cpu_t *cpu, u16 addr)
{
    cpu->D = false;
}

void CLI(cpu_t *cpu, u16 addr)
{
    cpu->I = false;
}

void CLV(cpu_t *cpu, u16 addr)
{
    cpu->V = false;
}

void BIT(cpu_t *cpu, u16 addr)
{
    u8 value = read_memory(cpu, addr);
    u8 result = cpu->A & value;

    cpu->Z = (result == 0);
    cpu->N = (value & NEGATIVE_FLAG) ? 1 : 0;
    cpu->V = (value & OVERFLOW_FLAG) ? 1 : 0;
}

void BRK(cpu_t *cpu, u16 addr)
{
    cpu->PC++;

    u16 return_addr = cpu->PC;

    write_memory(cpu, 0x100 | cpu->SP, (return_addr >> 8) & 0xFF); 
    cpu->SP--;

    write_memory(cpu, 0x100 | cpu->SP, return_addr & 0xFF);        
    cpu->SP--;

    u8 value = 0;
    value |= cpu->C ? CARRY_FLAG : 0;  
    value |= cpu->Z ? ZERO_FLAG : 0;  
    value |= cpu->I ? INTERRUPT_FLAG : 0;  
    value |= cpu->D ? DECIMAL_FLAG : 0;  
    value |= BREAK_FLAG;               
    value |= 0x20;               
    value |= cpu->V ? OVERFLOW_FLAG : 0;  
    value |= cpu->N ? NEGATIVE_FLAG: 0;  

    write_memory(cpu, (0x100 | cpu->SP), value);
    cpu->SP--;

    cpu->I = 1;
    cpu->PC = (read_memory(cpu, BRK_HANDLER)) | (read_memory(cpu, BRK_HANDLER+1) << 8);
}

void NOP(cpu_t *cpu, u16 addr)
{

}

opcode_t opcodes[256] = {
    [0xA9] = {IMM, 2, LDA}, // LDA Immediate
    [0xA5] = {ZP, 3, LDA},  // LDA Zero Page
    [0xB5] = {ZPX, 4, LDA}, // LDA Zero Page,X
    [0xAD] = {ABS, 4, LDA}, // LDA Absolute
    [0xBD] = {ABX, 4, LDA}, // LDA Absolute,X
    [0xB9] = {ABY, 4, LDA}, // LDA Absolute,Y
    [0xA1] = {IDX, 6, LDA}, // LDA (Indirect,X)
    [0xB1] = {IDY, 5, LDA}, // LDA (Indirect),Y

    [0xA2] = {IMM, 2, LDX}, // LDX Immediate
    [0xA6] = {ZP, 3, LDX},  // LDX Zero Page
    [0xB6] = {ZPY, 4, LDX}, // LDX Zero Page,Y
    [0xAE] = {ABS, 4, LDX}, // LDX Absolute
    [0xBE] = {ABY, 4, LDX}, // LDX Absolute,Y

    [0xA0] = {IMM, 2, LDY}, // LDY Immediate
    [0xA4] = {ZP, 3, LDY},  // LDY Zero Page
    [0xB4] = {ZPX, 4, LDY}, // LDY Zero Page,X
    [0xAC] = {ABS, 4, LDY}, // LDY Absolute
    [0xBC] = {ABX, 4, LDY}, // LDY Absolute,X

    [0x85] = {ZP, 3, STA},  // STA Zero Page
    [0x95] = {ZPX, 4, STA}, // STA Zero Page,X
    [0x8D] = {ABS, 4, STA}, // STA Absolute
    [0x9D] = {ABX, 5, STA}, // STA Absolute,X
    [0x99] = {ABY, 5, STA}, // STA Absolute,Y
    [0x81] = {IDX, 6, STA}, // STA (Indirect,X)
    [0x91] = {IDY, 6, STA}, // STA (Indirect),Y

    [0x86] = {ZP, 3, STX},  // STX Zero Page
    [0x96] = {ZPY, 4, STX}, // STX Zero Page,Y
    [0x8E] = {ABS, 4, STX}, // STX Absolute

    [0x84] = {ZP, 3, STY},  // STY Zero Page
    [0x94] = {ZPX, 4, STY}, // STY Zero Page,X
    [0x8C] = {ABS, 4, STY}, // STY Absolute

    [0x69] = {IMM, 2, ADC}, // ADC Immediate
    [0x65] = {ZP, 3, ADC},  // ADC Zero Page
    [0x75] = {ZPX, 4, ADC}, // ADC Zero Page,X
    [0x6D] = {ABS, 4, ADC}, // ADC Absolute
    [0x7D] = {ABX, 4, ADC}, // ADC Absolute,X
    [0x79] = {ABY, 4, ADC}, // ADC Absolute,Y
    [0x61] = {IDX, 6, ADC}, // ADC (Indirect,X)
    [0x71] = {IDY, 5, ADC}, // ADC (Indirect),Y

    [0xE9] = {IMM, 2, SBC}, // SBC Immediate
    [0xE5] = {ZP, 3, SBC},  // SBC Zero Page
    [0xF5] = {ZPX, 4, SBC}, // SBC Zero Page,X
    [0xED] = {ABS, 4, SBC}, // SBC Absolute
    [0xFD] = {ABX, 4, SBC}, // SBC Absolute,X
    [0xF9] = {ABY, 4, SBC}, // SBC Absolute,Y
    [0xE1] = {IDX, 6, SBC}, // SBC (Indirect,X)
    [0xF1] = {IDY, 5, SBC}, // SBC (Indirect),Y

    [0x29] = {IMM, 2, AND}, // AND Immediate
    [0x25] = {ZP, 3, AND},  // AND Zero Page
    [0x35] = {ZPX, 4, AND}, // AND Zero Page,X
    [0x2D] = {ABS, 4, AND}, // AND Absolute
    [0x3D] = {ABX, 4, AND}, // AND Absolute,X
    [0x39] = {ABY, 4, AND}, // AND Absolute,Y
    [0x21] = {IDX, 6, AND}, // AND (Indirect,X)
    [0x31] = {IDY, 5, AND}, // AND (Indirect),Y

    [0x49] = {IMM, 2, EOR}, // EOR Immediate
    [0x45] = {ZP, 3, EOR},  // EOR Zero Page
    [0x55] = {ZPX, 4, EOR}, // EOR Zero Page,X
    [0x4D] = {ABS, 4, EOR}, // EOR Absolute
    [0x5D] = {ABX, 4, EOR}, // EOR Absolute,X
    [0x59] = {ABY, 4, EOR}, // EOR Absolute,Y
    [0x41] = {IDX, 6, EOR}, // EOR (Indirect,X)
    [0x51] = {IDY, 5, EOR}, // EOR (Indirect),Y

    [0x09] = {IMM, 2, ORA}, // ORA Immediate
    [0x05] = {ZP, 3, ORA},  // ORA Zero Page
    [0x15] = {ZPX, 4, ORA}, // ORA Zero Page,X
    [0x0D] = {ABS, 4, ORA}, // ORA Absolute
    [0x1D] = {ABX, 4, ORA}, // ORA Absolute,X
    [0x19] = {ABY, 4, ORA}, // ORA Absolute,Y
    [0x01] = {IDX, 6, ORA}, // ORA (Indirect,X)
    [0x11] = {IDY, 5, ORA}, // ORA (Indirect),Y

    [0xC9] = {IMM, 2, CMP}, // CMP Immediate
    [0xC5] = {ZP, 3, CMP},  // CMP Zero Page
    [0xD5] = {ZPX, 4, CMP}, // CMP Zero Page,X
    [0xCD] = {ABS, 4, CMP}, // CMP Absolute
    [0xDD] = {ABX, 4, CMP}, // CMP Absolute,X
    [0xD9] = {ABY, 4, CMP}, // CMP Absolute,Y
    [0xC1] = {IDX, 6, CMP}, // CMP (Indirect,X)
    [0xD1] = {IDY, 5, CMP}, // CMP (Indirect),Y

    [0xE0] = {IMM, 2, CPX}, // CPX Immediate
    [0xE4] = {ZP, 3, CPX},  // CPX Zero Page
    [0xEC] = {ABS, 4, CPX}, // CPX Absolute

    [0xC0] = {IMM, 2, CPY}, // CPY Immediate
    [0xC4] = {ZP, 3, CPY},  // CPY Zero Page
    [0xCC] = {ABS, 4, CPY}, // CPY Absolute

    [0x0A] = {IMP, 2, ASL}, // ASL Accumulator
    [0x06] = {ZP, 5, ASL},  // ASL Zero Page
    [0x16] = {ZPX, 6, ASL}, // ASL Zero Page,X
    [0x0E] = {ABS, 6, ASL}, // ASL Absolute
    [0x1E] = {ABX, 7, ASL}, // ASL Absolute,X

    [0x4A] = {IMP, 2, LSR}, // LSR Accumulator
    [0x46] = {ZP, 5, LSR},  // LSR Zero Page
    [0x56] = {ZPX, 6, LSR}, // LSR Zero Page,X
    [0x4E] = {ABS, 6, LSR}, // LSR Absolute
    [0x5E] = {ABX, 7, LSR}, // LSR Absolute,X

    [0x2A] = {IMP, 2, ROL}, // ROL Accumulator
    [0x26] = {ZP, 5, ROL},  // ROL Zero Page
    [0x36] = {ZPX, 6, ROL}, // ROL Zero Page,X
    [0x2E] = {ABS, 6, ROL}, // ROL Absolute
    [0x3E] = {ABX, 7, ROL}, // ROL Absolute,X

    [0x6A] = {IMP, 2, ROR}, // ROR Accumulator
    [0x66] = {ZP, 5, ROR},  // ROR Zero Page
    [0x76] = {ZPX, 6, ROR}, // ROR Zero Page,X
    [0x6E] = {ABS, 6, ROR}, // ROR Absolute
    [0x7E] = {ABX, 7, ROR}, // ROR Absolute,X

    [0x90] = {REL, 2, BCC}, // BCC Relative
    [0xB0] = {REL, 2, BCS}, // BCS Relative
    [0xF0] = {REL, 2, BEQ}, // BEQ Relative
    [0xD0] = {REL, 2, BNE}, // BNE Relative
    [0x30] = {REL, 2, BMI}, // BMI Relative
    [0x10] = {REL, 2, BPL}, // BPL Relative
    [0x50] = {REL, 2, BVC}, // BVC Relative
    [0x70] = {REL, 2, BVS}, // BVS Relative

    [0x4C] = {ABS, 3, JMP}, // JMP Absolute
    [0x6C] = {IND, 5, JMP}, // JMP Indirect
    [0x20] = {ABS, 6, JSR}, // JSR Absolute
    [0x60] = {IMP, 6, RTS}, // RTS Implied
    [0x40] = {IMP, 6, RTI}, // RTI Implied

    [0xE6] = {ZP, 5, INC},  // INC Zero Page
    [0xF6] = {ZPX, 6, INC}, // INC Zero Page,X
    [0xEE] = {ABS, 6, INC}, // INC Absolute
    [0xFE] = {ABX, 7, INC}, // INC Absolute,X

    [0xE8] = {IMP, 2, INX}, // INX Implied
    [0xC8] = {IMP, 2, INY}, // INY Implied

    [0xC6] = {ZP, 5, DEC},  // DEC Zero Page
    [0xD6] = {ZPX, 6, DEC}, // DEC Zero Page,X
    [0xCE] = {ABS, 6, DEC}, // DEC Absolute
    [0xDE] = {ABX, 7, DEC}, // DEC Absolute,X

    [0xCA] = {IMP, 2, DEX}, // DEX Implied
    [0x88] = {IMP, 2, DEY}, // DEY Implied

    [0x24] = {ZP, 3, BIT},  // BIT Zero Page
    [0x2C] = {ABS, 4, BIT}, // BIT Absolute

    [0x38] = {IMP, 2, SEC}, // SEC Implied
    [0xF8] = {IMP, 2, SED}, // SED Implied
    [0x78] = {IMP, 2, SEI}, // SEI Implied
    [0x18] = {IMP, 2, CLC}, // CLC Implied
    [0xD8] = {IMP, 2, CLD}, // CLD Implied
    [0x58] = {IMP, 2, CLI}, // CLI Implied
    [0xB8] = {IMP, 2, CLV}, // CLV Implied

    [0x48] = {IMP, 3, PHA}, // PHA Implied
    [0x08] = {IMP, 3, PHP}, // PHP Implied
    [0x68] = {IMP, 4, PLA}, // PLA Implied
    [0x28] = {IMP, 4, PLP}, // PLP Implied

    [0xAA] = {IMP, 2, TAX}, // TAX Implied
    [0xA8] = {IMP, 2, TAY}, // TAY Implied
    [0x8A] = {IMP, 2, TXA}, // TXA Implied
    [0x98] = {IMP, 2, TYA}, // TYA Implied
    [0xBA] = {IMP, 2, TSX}, // TSX Implied
    [0x9A] = {IMP, 2, TXS}, // TXS Implied

    [0x00] = {IMP, 7, BRK}, // BRK Implied
    [0xEA] = {IMP, 2, NOP}, // NOP Implied
};