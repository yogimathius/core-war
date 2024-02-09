#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "../include/vm.h"

core_t *init_vm() {
    core_t *core = malloc(sizeof(core_t));
    memset(core->memory, 0, MEM_SIZE);
    memset(core->registers, 0, REG_NUMBER);
    core->champion_count = 0;
    core->instruction_pointer = 0;

    return core;
}

int read_vm(core_t *core_t, int address) {
    if (address < REG_NUMBER) {
        return core_t->registers[address];
    } else if (address < MEM_SIZE) {
        return core_t->memory[address];
    } else {
        printf("Error: Invalid memory address\n");
        return 1;
    }
}

void write_vm(core_t *core_t, int address, int value) {
    if (address < REG_NUMBER) {
        core_t->registers[address] = value;
    } else if (address < MEM_SIZE) {
        core_t->memory[address] = value;
    } else {
        printf("Error: Invalid memory address\n");
        // return 1;
    }
}