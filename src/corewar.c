#include <stdio.h>
#include <stdint.h>
#include "../include/op.h"
#include "../include/vm.h"

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, unsigned long operand) {
    const op_t *operation = &op_tab[opcode]; 
    
    if (operation->inst != NULL) {
        printf("calling operation: %s\n", operation->mnemonique);
        // TODO: this might not be the correct way to pass operand??
        operation->inst(champ, vm, opcode, (int *) &operand);
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}

// TODO: are the vm and champs passed all the way down as pointers???
void run_program(core_t *vm, champion_t champ, unsigned long *program, int program_size) {
    for (int i = 0; i < program_size; i++) {
        unsigned long instruction = program[i];
        enum op_types opcode = (enum op_types)(instruction >> 60);
        unsigned long operand = instruction & 0xFFFFFFFFFFFFFFF; 

        execute_instruction(vm, &champ, opcode, operand);
        vm->instruction_pointer++;
    }
}

uint64_t generate_instruction(int opcode, int argument) {
    return ((uint64_t)opcode << 60) | ((uint64_t)argument & 0xFFFFFFFFFFFFFFF);
}

int main() {
    core_t vm;
    vm.instruction_pointer = 0;

    // NOTE: just for testing purposes, we might not be using the same format but this is specifically for testing the control flow of executing instructions
    unsigned long program[] = {
        // arguments are random immediate values and could mean nothing XD
        generate_instruction(OP_ADD, 0x5678ULL),   
        generate_instruction(OP_SUB, 0x5678ULL), 
        generate_instruction(OP_AND, 0xABCDULL), 
        generate_instruction(OP_OR, 0xEF01ULL),  
        generate_instruction(OP_XOR, 0x2345ULL), 
        generate_instruction(OP_ZJMP, 0x3456ULL), 
        generate_instruction(OP_LDI, 0x6789ULL), 
        generate_instruction(OP_STI, 0x789AULL), 
        generate_instruction(OP_FORK, 0x89ABULL),
        generate_instruction(OP_LLD, 0xBCDEULL),  
        generate_instruction(OP_LLDI, 0xCDEFULL),
        generate_instruction(OP_LFORK, 0xDEAFULL),
        generate_instruction(OP_AFF, 0xFADEULL),
        generate_instruction(OP_NOTHING, 0x0000ULL),
    };

    int program_size = sizeof(program) / sizeof(program[0]);
    // TODO: replace program with champion when ready!!
    champion_t *champ = init_champion(1);
    run_program(&vm, *champ, program, program_size);

    return 0;
}
