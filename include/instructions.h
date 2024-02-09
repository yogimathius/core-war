#ifndef _PARSE_INSTRUCTIONS_H
#define _PARSE_INSTRUCTIONS_H
#include "op.h"

char **parse_instructions(char *instruction);

void build_instructions(champion_t *champ, char **instructions, instruction_t **inst_ptr);

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *instruction);

#endif