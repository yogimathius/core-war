#ifndef VM_H_
#define VM_H_

#include "champion.h"

#define MEM_SIZE (6 * 1024)
#define REG_NUMBER 16

#define MAX_CHAMPIONS 4  // Maximum number of champions

core_t *init_vm();

int read_vm(core_t *core_vm, int address);

void write_vm(core_t *core_vm, int address, int value);

void load_instructions(core_t *core);

void load_instructionsv2(core_t *core);

void build_processes(core_t *core);

#endif
