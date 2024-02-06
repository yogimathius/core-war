#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/op.h"
#include "../include/vm.h"
#include "../include/instructions.h"


// TODO: are the vm and champs passed all the way down as pointers???
void run_program(core_t *vm, champion_t champ) {
    UNUSED(vm);
    printf("Running program: %s\n", champ.header.prog_name);

    char *instructions = champ.instructions;
    instruction_t *inst = (instruction_t*)malloc(sizeof(instruction_t));

    char **parsed_instructions = parse_instruction(instructions);

    build_instructions(&champ, parsed_instructions, inst);
    while (inst->opcode != -1) {
        printf("Executing instruction: %s\n", op_tab[inst->opcode].mnemonique);
        int operands[op_tab[inst->opcode].nbr_args];
        for (int i = 0; i < op_tab[inst->opcode].nbr_args; i++) {
            operands[i] = atoi(&inst->operands[i]);
        }
        execute_instruction(vm, &champ, inst->opcode, operands);
        inst++;
    }
    free(parsed_instructions);
}

int main(int ac, char **av) {
    printf("Starting corewar\n");
    core_t vm;
    vm.instruction_pointer = 0;
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }
    while (ac > 1) {
        printf("Loading champion: %s\n", av[ac - 1]);
        champion_t *champ = init_champion();
        create_champion(champ, av[ac - 1]);
        add_champion(&vm, champ);
        ac--;
    }

    int i = 0;
    while (i < vm.champion_count) {
        printf("Champion P%d: %s\n", vm.champions[i].id, vm.champions[i].header.prog_name);
        run_program(&vm, vm.champions[i]);
        i++;
    }
    // run_program(&vm, *champ, program, program_size);

    return 0;
}
