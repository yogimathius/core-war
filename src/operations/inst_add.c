#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Takes 3 parameters: 3 registers. Add the first to the second, and store
the result to the third. Modifies the carry.
*/
int inst_add(champion_t *champion, core_t *core, code_t code, int *inst) {
    log_instruction_args(champion, core, code, inst);

    int operand1 = champion->registers[inst[0]];
    int operand2 = champion->registers[inst[1]];
    int destination_register_index = inst[2];

    int result = operand1 + operand2;

    champion->registers[destination_register_index] = result;

    if (result == 0) {
        champion->carry_flag = 1;
    } else {
        champion->carry_flag = 0;
    }

    return 0;
};
