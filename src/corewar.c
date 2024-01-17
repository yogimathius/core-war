#include <stdio.h>
#include <stdint.h>

#define MEMORY_SIZE 256
#define NUM_REGISTERS 16

typedef struct {
    uint64_t memory[MEMORY_SIZE];
    uint64_t registers[NUM_REGISTERS];
    uint64_t instruction_pointer;
} VirtualMachine;

typedef enum {
    MOV,
    ADD,
    SUB,
    HALT,
} Instruction;

void execute_instruction(VirtualMachine *vm, Instruction opcode, uint64_t operand) {
    switch (opcode) {
        case MOV:
            vm->registers[0] = operand;
            break;
        case ADD:
            vm->registers[0] += operand;
            break;
        case SUB:
            vm->registers[0] -= operand;
            break;
        case HALT:
            printf("Halted.\n");
            // In a real Core War environment, this might trigger removal of the program
            break;
        default:
            printf("Unknown opcode\n");
            // Handle unknown opcode error
    }
}

void run_program(VirtualMachine *vm, uint64_t *program, size_t program_size) {
    while (vm->instruction_pointer < program_size) {
        uint64_t instruction = program[vm->instruction_pointer];
        Instruction opcode = (Instruction)(instruction >> 60);
        uint64_t operand = instruction & 0xFFFFFFFFFFFFFFF; // Extract lower 60 bits

        execute_instruction(vm, opcode, operand);
        vm->instruction_pointer++;
    }
}

int main() {
    VirtualMachine vm;
    vm.instruction_pointer = 0;

    // Example program: MOV 10, ADD 5, HALT
    uint64_t program[] = {
        0xC00000000000000A,  // MOV 10
        0x8000000000000005,  // ADD 5
        0xA000000000000000   // HALT
    };

    size_t program_size = sizeof(program) / sizeof(program[0]);

    run_program(&vm, program, program_size);

    return 0;
}