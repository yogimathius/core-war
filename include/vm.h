#ifndef VM_H_
#define VM_H_

#include "champion.h"

#define MEM_SIZE (6 * 1024)
#define REG_NUMBER 16

#define MAX_CHAMPIONS 4  // Maximum number of champions

void initializeVirtualMachine(core_t *virtualMachine);

int readVirtualMachine(core_t *virtualMachine, int address);

void writeVirtualMachine(core_t *virtualMachine, int address, int value);

#endif
