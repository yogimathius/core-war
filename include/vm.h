#ifndef VM_H_
#define VM_H_

#define MEM_SIZE (6 * 1024)
#define REG_NUMBER 16

typedef struct {
    int cells[MEM_SIZE];
    int registers[REG_NUMBER];
} VirtualMachine;

void initializeVirtualMachine(VirtualMachine *virtualMachine);

int readVirtualMachine(VirtualMachine *virtualMachine, int address);

void writeVirtualMachine(VirtualMachine *virtualMachine, int address, int value);

#endif
