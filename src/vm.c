#include "stdio.h"
#include "../include/vm.h"

void initializeVirtualMachine(VirtualMachine *VirtualMachine) {
    for (int i = 0; i < MEM_SIZE; i++) {
        VirtualMachine->cells[i] = 0;
    }

    for (int i = 0; i < REG_NUMBER; i++) {
        VirtualMachine->registers[i] = 0;
    }
}

int readVirtualMachine(VirtualMachine *VirtualMachine, int address) {
    if (address < REG_NUMBER) {
        return VirtualMachine->registers[address];
    } else if (address < MEM_SIZE) {
        return VirtualMachine->cells[address];
    } else {
        printf("Error: Invalid memory address\n");
        return 1;
    }
}

void writeVirtualMachine(VirtualMachine *VirtualMachine, int address, int value) {
    if (address < REG_NUMBER) {
        VirtualMachine->registers[address] = value;
    } else if (address < MEM_SIZE) {
        VirtualMachine->cells[address] = value;
    } else {
        printf("Error: Invalid memory address\n");
        // return 1;
    }
}