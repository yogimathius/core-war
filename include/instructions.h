#ifndef _PARSE_INSTRUCTIONS_H
#define _PARSE_INSTRUCTIONS_H

#include "op.h"
#include "champion.h"

void parse_instructions(const char *hex_buffer, int bytes_read, champion_t *champion);

int build_instructions(char **instructions, champion_t *champion);

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst);

int load_value_from_address(core_t *core, int address);

void run_instructions(core_t *core_vm);

#endif