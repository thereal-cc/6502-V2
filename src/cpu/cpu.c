#include "cpu.h"
#include "instruction.h"

void cpu_init(cpu_t *cpu)
{
    // Clear Memory
    memset(cpu->memory, 0, sizeof(cpu->memory));

    // Patch vectors (last 6 bytes of memory)
    // NMI vector -> load_addr
    cpu->memory[INTERRUPT_HANDLER] = 0;        // low byte
    cpu->memory[INTERRUPT_HANDLER + 1] = 80;    // high byte

    // RESET vector -> load_addr
    cpu->memory[RESET_LOW] = 0;
    cpu->memory[RESET_HIGH] = 80;

    // IRQ/BRK vector -> load_addr
    cpu->memory[BRK_HANDLER] = 0;
    cpu->memory[BRK_HANDLER + 1] = 80;

    // Registers
    cpu->SP = 0xFD;
    cpu->A = cpu->X = cpu->Y = 0;

    // Status flags
    cpu->N = cpu->V = cpu->B = cpu->D = cpu->Z = cpu->C = false;
    cpu->I = true; // interrupts disabled on reset

    cpu->running = true;
    cpu->global_cycles = 0;
}

void cpu_cycle(cpu_t *cpu)
{
    u8 opcode_byte = read_memory(cpu, cpu->PC++);
    opcode_t opcode = opcodes[opcode_byte];
    u16 addr;

    switch(opcode.addr_mode) {
        case IMM:
            addr = imm_address(cpu);
            break;
        case ZP:
            addr = zp_address(cpu);
            break;
        case ZPX:
            addr = zpx_address(cpu);
            break;
        case ZPY:
            addr = zpy_address(cpu);
            break;
        case ABS:
            addr = abs_address(cpu);
            break;
        case ABX:
            addr = abx_address(cpu);
            break;
        case ABY:
            addr = aby_address(cpu);
            break;
        case IND:
            addr = ind_address(cpu);
            break;
        case IDX:
            addr = indx_address(cpu);
            break;
        case IDY:
            addr = indy_address(cpu);
            break;
        case IMP:
            addr = imp_address(cpu);
            break;
        case REL:
            addr = rel_address(cpu);
            break;
    }

    opcode.operation(cpu, addr);

    cpu->global_cycles += opcode.cycles;
}

u8 load_program(cpu_t *cpu, const char* rom_path, u16 load_addr)
{
    u8 status = 1;

    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL)
    {
        return status;
    }


    size_t bytes_read = fread(cpu->memory + load_addr, sizeof(u8), sizeof(cpu->memory) - load_addr, fptr);
    fclose(fptr);

    // Nothing Loaded
    if (bytes_read == 0) return status;

    write_memory(cpu, INTERRUPT_HANDLER, 0x00);
    write_memory(cpu, INTERRUPT_HANDLER + 1, 0x0F);

    write_memory(cpu, RESET_LOW, 0x00);
    write_memory(cpu, RESET_HIGH, 0xFF);

    write_memory(cpu, BRK_HANDLER, 0x00);
    write_memory(cpu, BRK_HANDLER + 1, 0x00);

    cpu->PC = load_addr;

    status = 0;
    return status;
}

u8 read_memory(cpu_t *cpu, u16 address)
{
    return cpu->memory[address & 0xFFFF];
}

void write_memory(cpu_t *cpu, u16 address, u8 value)
{
    cpu->memory[address & 0xFFFF] = value;
}

void N_flag(cpu_t *cpu, u8 value)
{
    cpu->N = (value & 0x80) != 0; 
}

void V_flag(cpu_t *cpu, u8 value, u8 result)
{
    cpu->V = (~(cpu->A ^ value) & (cpu->A ^ result) & 0x80) != 0;
}

void Z_flag(cpu_t *cpu, u8 value)
{
    cpu->Z = ((value & 0xFF) == 0);
}

void C_flag(cpu_t *cpu, u8 value)
{
    cpu->C = (value > 0xFF);
}

void cpu_display_registers(cpu_t *cpu) {
    u8 value = 0;

    value |= cpu->C ? 0x01 : 0;  
    value |= cpu->Z ? 0x02 : 0;  
    value |= cpu->I ? 0x04 : 0;  
    value |= cpu->D ? 0x08 : 0;  
    value |= 0x10;               
    value |= 0x20;               
    value |= cpu->V ? 0x40 : 0;  
    value |= cpu->N ? 0x80 : 0;  

    printf("A: %02X, X: %02X, Y: %02X, PC: %04X, SP: %02X, SR: %02X\n",
               cpu->A, cpu->X, cpu->Y, cpu->PC, cpu->SP, value);
}

void print_memory(cpu_t *cpu, u16 start, u16 end) {
    printf("Memory dump from $%04X to $%04X:\n", start, end);
    for (u16 addr = start; addr <= end; addr++) {
        // Print 16 bytes per line
        if ((addr - start) % 16 == 0) {
            printf("\n%04X: ", addr); // Print address label at the start of each line
        }
        printf("%02X ", read_memory(cpu, addr));
    }
    printf("\n\n");
}