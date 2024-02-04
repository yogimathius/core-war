#include "stdio.h"
#include "../include/vm.h"

void init_vm(core_t *core_t) {
    for (int i = 0; i < MEM_SIZE; i++) {
        core_t->memory[i] = 0;
    }

    for (int i = 0; i < REG_NUMBER; i++) {
        core_t->registers[i] = 0;
    }
    core_t->champion_count = 0;
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