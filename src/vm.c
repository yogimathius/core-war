#include "stdio.h"
#include "../include/vm.h"

void initializeVirtualMachine(virtual_machine_t *virtual_machine_t) {
    for (int i = 0; i < MEM_SIZE; i++) {
        virtual_machine_t->memory[i] = 0;
    }

    for (int i = 0; i < REG_NUMBER; i++) {
        virtual_machine_t->registers[i] = 0;
    }
}

int readVirtualMachine(virtual_machine_t *virtual_machine_t, int address) {
    if (address < REG_NUMBER) {
        return virtual_machine_t->registers[address];
    } else if (address < MEM_SIZE) {
        return virtual_machine_t->memory[address];
    } else {
        printf("Error: Invalid memory address\n");
        return 1;
    }
}

void writeVirtualMachine(virtual_machine_t *virtual_machine_t, int address, int value) {
    if (address < REG_NUMBER) {
        virtual_machine_t->registers[address] = value;
    } else if (address < MEM_SIZE) {
        virtual_machine_t->memory[address] = value;
    } else {
        printf("Error: Invalid memory address\n");
        // return 1;
    }
}