#ifndef _PARSE_INSTRUCTIONS_H
#define _PARSE_INSTRUCTIONS_H
#include "op.h"

char **parse_instructions(const char *hex_buffer, int bytes_read, champion_t *champion);

int build_instructions(char **instructions, champion_t *champion);

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *instruction);

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst);

int load_value_from_address(core_t *core, int address);

#endif