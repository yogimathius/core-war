// From tutorial:
// https://www.andreinc.net/2021/12/01/writing-a-simple-vm-in-less-than-125-lines-of-c#the-main-memory

#include <stdio.h>
#include <stdint.h>

// Operation Codes, instructions that programs can execute
#define OPC(i) ((i)>>12)
#define NOPS (16) // number of instructions

// Actual Operation, which executes when an operation code is read?
typedef void (*op_ex_f)(uint16_t instruction);

// Specific operation instructions
static inline void add(uint16_t i)  { /* code here */ }
static inline void and(uint16_t i)  { /* code here */ }


// Array of instructions
op_ex_f op_ex[NOPS] = { 
    // br, add, ld, st, jsr, and, ldr, str, rti, not, ldi, sti, jmp, res, lea, trap 
};

uint16_t PC_START = 0x3000;
uint16_t mem[UINT16_MAX+1] = {0};

enum regist { 
  R0 = 0, R1, R2, R3, R4, R5, R6, R7, RPC, RCND, RCNT
};

uint16_t reg[RCNT] = {0};

static inline uint16_t read_memory(uint16_t address) { 
  return mem[address];
}

static inline void write_memory(uint16_t address, uint16_t val) { 
  mem[address] = val;
}

enum flags { FP = 1 << 0, FZ = 1 << 1, FN = 1 << 2 };

// Why is this called uf??
static inline void uf(enum regist r) {
    if (reg[r]==0) reg[RCND] = FZ;       // the value in r is zero
    else if (reg[r]>>15) reg[RCND] = FN; // the value in r is z negative number
    else reg[RCND] = FP;                 // the value in r is a positive number
}