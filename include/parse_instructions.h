#ifndef _PARSE_INSTRUCTIONS_H
#define _PARSE_INSTRUCTIONS_H
#include "op.h"

typedef struct instruction {
    int opcode;
    char *operands;
    struct instruction *next;
} instruction_t;

char **parse_instruction(char *instruction);

void build_instructions(champion_t *champ, char **instructions, instruction_t *inst);

#endif