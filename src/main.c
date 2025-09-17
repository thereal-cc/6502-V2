#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[]) {
    cpu_t cpu;
    cpu_init(&cpu);

    char *rompath = (argc != 2) ? (printf("No File Loaded, Using Test Rom\n"), "tests/Fibonacci.bin") : argv[1];
    u8 status = load_program(&cpu, rompath, 0x00);
    if (status == 1) {
        fprintf(stderr, "Error: Could not load ROM\n");
        return EXIT_FAILURE;
    }

    // CPU Clock Cycle
    while (cpu.running) {
        cpu_cycle(&cpu);
        cpu_display_registers(&cpu);
    }

    // print_memory(&cpu, 0x00, 0xFF);

    return EXIT_SUCCESS;
}