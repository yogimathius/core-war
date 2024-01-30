#ifndef VM_H_
#define VM_H_

#include "champion.h"

#define MEM_SIZE (6 * 1024)
#define REG_NUMBER 16

#define MAX_CHAMPIONS 4  // Maximum number of champions

typedef struct {
    int memory[MEM_SIZE];
    int registers[REG_NUMBER];
    int instruction_pointer;
    int cycle;
    int champion_count;
    champion_t champions[MAX_CHAMPIONS];
} virtual_machine_t;


void initializeVirtualMachine(virtual_machine_t *virtualMachine);

int readVirtualMachine(virtual_machine_t *virtualMachine, int address);

void writeVirtualMachine(virtual_machine_t *virtualMachine, int address, int value);

#endif
