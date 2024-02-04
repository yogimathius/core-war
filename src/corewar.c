#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../include/op.h"
#include "../include/vm.h"

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *instruction) {
    const op_t *operation = &op_tab[opcode]; 
    
    if (operation->inst != NULL) {
        printf("calling operation: %s\n", operation->mnemonique);
        // TODO: this might not be the correct way to pass operand??
        operation->inst(champ, vm, opcode, instruction);
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}

int *parse_instruction(char instruction[64]) {
    int *parsed_instruction = (int*)malloc(strlen(instruction) * sizeof(int));
    char *token = strtok(instruction, " ");
    int i = 0;
    while (token != NULL) {
        parsed_instruction[i] = atoi(token);
        token = strtok(NULL, " ");
        i++;
    }
    return parsed_instruction;
}

// TODO: are the vm and champs passed all the way down as pointers???
void run_program(core_t *vm, champion_t champ, char program[15][64], int program_size) {
    for (int i = 0; i < program_size; i++) {
        char opcode_hex[2];
        strncpy(opcode_hex, program[i], 2);
        printf("opcode_hex: %s\n", opcode_hex);

        enum op_types opcode = (enum op_types)strtol(opcode_hex, NULL, 16);
        char instruction[64];
        strncpy(instruction, program[i] + 3, 64);
        printf("instruction: %s\n", instruction);
        int *parsed_instruction = parse_instruction(instruction);
        execute_instruction(vm, &champ, opcode, parsed_instruction);
        // vm->instruction_pointer++;
    }
    UNUSED(program);
    UNUSED(champ);
    UNUSED(vm);
    UNUSED(program_size);
}

uint64_t generate_instruction(int opcode, int argument) {
    return ((uint64_t)opcode << 60) | ((uint64_t)argument & 0xFFFFFFFFFFFFFFF);
}

int main() {
    core_t vm;
    vm.instruction_pointer = 0;

    char program_two[15][64] = {
        "02 90 00 00 00 22 03",
        "03 70 04 00 00 00 22",
        "04 54 01 01 03",
		"05 54 04 05 06",
		"06 54 04 05 06",
		"07 54 06 07 12",
		"08 54 12 14 15",
		"09 80 00 00 00 13",
		"0a 64 3 00 00 00 00 04 01",
		"0b 90 00 00 00 22 03",
		"0c 80 00 00 00 1",
		"0d D0 01 1C 03",
		"0e A4 00 00 00 00 03 00 00 00 04 01",
		"0f 80 00 00 00 02",
		"010 40	01"
    };

    // // TODO: replace program with champion when ready!!
    const champion_t *champ = init_champion(1);

    int program_two_size = sizeof(program_two) / sizeof(program_two[0]);
    // TODO: replace program_two with champion when ready!!
    run_program(&vm, *champ, program_two, program_two_size);

    return 0;
}
