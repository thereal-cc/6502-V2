#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[]) {
    cpu_t cpu;
    cpu_init(&cpu);

    char *rompath;
    if (argc != 2) {
        printf("No File Loaded, Using Test Rom\n");
        rompath = "tests/Fibonacci.bin";
     } else {
        rompath = argv[1];
     } 
    u8 status = load_program(&cpu, rompath, 0x0600);
    if (status == 1) {
        fprintf(stderr, "Error: Could not load ROM\n");
        return EXIT_FAILURE;
    }

    // CPU Clock Cycle
    while (cpu.running) {
        cpu_display_registers(&cpu);
        cpu_cycle(&cpu);
    }

    print_memory(&cpu, 0x00, 0xFF);

    return EXIT_SUCCESS;
}