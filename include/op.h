#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef _OP_H_
#define _OP_H_
#define UNUSED(x) (void)(x)

// Modulo of the index
#define IDX_MOD 512

// Maximum number of arguments an operation can take
#define MAX_ARGS_NUMBER 4

// Argument types
typedef char args_type_t;

// Byte code type
typedef unsigned char code_t;

// Enumeration of parameter types
enum parameter_types { T_REG = 1, T_DIR = 2, T_IND = 4, T_LAB = 8 };

#define MEM_SIZE (6 * 1024)
#define REG_NUMBER 16

#define MAX_CHAMPIONS 4  

// Enumeration of operation types
enum op_types {
  OP_LIVE, // 0X01
  OP_LD, // 0X02
  OP_ST, // 0X03
  OP_ADD, // 0X04
  OP_SUB, // 0X05
  OP_AND, // 0X06
  OP_OR, // 0X07
  OP_XOR, // 0X08
  OP_ZJMP, // 0X09
  OP_LDI, // 0X0A
  OP_STI, // 0X0B
  OP_FORK, // 0X0C
  OP_LLD, // 0X0D
  OP_LLDI, // 0X0E
  OP_LFORK, // 0X0F
  OP_AFF, // 0X10
  OP_NOTHING, // 0X11
  OP_NB // 0X12
};

// Size of indirect addressing mode in bytes
#define IND_SIZE 2

// Size of direct addressing mode in bytes
#define DIR_SIZE 4

// Size of registers in bytes
#define REG_SIZE DIR_SIZE

// Header structure for the program
#define PROG_NAME_LENGTH 128
#define COMMENT_LENGTH 2048
#define COREWAR_EXEC_MAGIC 0xea83f3

enum registers {
  r1 = 0,
  r2,
  r3,
  r4,
  r5,
  r6,
  r7,
  r8,
  r9,
  r10,
  r11,
  r12,
  r13,
  r14,
  r15,
  r16
};

// Header structure for the program
typedef struct header_s {
  int magic; 
  char prog_name[PROG_NAME_LENGTH + 1];
  int prog_size;
  char comment[COMMENT_LENGTH + 1];
} header_t;

typedef struct operand {
    int type; 
    int value;
    char *label;
    int index;
} operand_t;

typedef struct instruction {
    int opcode;
    int *operands;
    operand_t *operand_list; 
    int num_operands;
    struct instruction *next;
} instruction_t;

typedef struct champion {
  int id;
  header_t header;
  enum registers registers[REG_NUMBER];
  int counter;
  int carry_flag;
  instruction_t inst[MEM_SIZE + 1];
  int instruction_size;
  instruction_t *instruction_list;
  char **parsed_instructions;
  int parsed_instructions_size;
  int color;
  int lives;
  int dead;
} champion_t;

// Structure representing the core of the virtual machine
typedef struct core_s {
    int memory[MEM_SIZE];
    char *hex_memory[MEM_SIZE];
    int instruction_pointer;
    int nbr_cycles;
    int cycle_to_die;
    int champion_count;
    champion_t champions[MAX_CHAMPIONS];
    int lives;
    int winner;
} core_t;

// Structure representing an operation

typedef struct op_s {
  char *mnemonique;     // Operation mnemonic
  char nbr_args;        // Number of arguments
  args_type_t type[MAX_ARGS_NUMBER];  // Types of arguments
  char code;            // Operation code
  int nbr_cycles;       // Number of cycles for the operation
  int (*inst)(champion_t *, core_t *, code_t, int *);
} op_t;

typedef struct op_s op_t;

// Array of operation structures

extern const op_t op_tab[];

// Maximum number of champions
#define MAX_CHAMPIONS 4

// Number of cycles before a process is declared dead
#define CYCLE_TO_DIE 1536

// Cycle delta
#define CYCLE_DELTA 5

// Number of lives required to prevent a process from being declared dead
#define NBR_LIVE 40

// Implement me

/* 
Takes 1 parameter: 4 bytes that represent the player's number.
Indicates the player is alive.
*/
int inst_live(champion_t *, core_t *, code_t, int *);

/* 
Takes 2 parameters: loads the first parameter to the second parameter.
Second parameter is a register. Ex: ld 34, r3 loads the REG_SIZE bytes
starting from the Counter + 34 % IDX_MOD into r3.
*/
int inst_ld(champion_t *, core_t *, code_t, int *);

/* 
Takes 2 parameters: Stores first parameter (a register) into the second
parameter (it can be another register or a number). Ex: st r4, 34 stores
r4 at the address of Counter + 34 % IDX_MOD; st r3, r8 copies r3 into
r8.
*/
int inst_st(champion_t *, core_t *, code_t, int *);

/* 
Takes 3 parameters: 3 registers. Add the first to the second, and store
the result to the third. Modifies the carry.
*/
int inst_add(champion_t *, core_t *, code_t, int *);

/* 
Same as add, but Subtracting. Modifies the carry.
*/
int inst_sub(champion_t *, core_t *, code_t, int *);

/* 
Same as add, and sub, but does a binary operation AND between the
first and the second, and storing the result in the third parameter.
Modifies the carry.
*/
int inst_and(champion_t *, core_t *, code_t, int *);

/* 
Same as and, but performing an OR.
*/
int inst_or(champion_t *, core_t *, code_t, int *);

/* 
Same as and and or, but performing an XOR.
*/
int inst_xor(champion_t *, core_t *, code_t, int *);

/* 
Takes 1 parameter, that must be an index. Jumps to this index if carry is
1. Else, does nothing, but still consume time.
Zjmp %23 -> if carry == 1, Counter + 23 % IDX_MOD to Counter
*/
int inst_zjmp(champion_t *, core_t *, code_t, int *);

/* 
Takes 3 parameters. First two are indexes and the third one is a register
Ex: ldi 3, %4, r1 -> reads IND_SIZ bytes from address Counter + 3 %
IDX_MOD, add 4 to this value (SumResult). REG_SIZE byte are read
from the Counter + SumResult % IDX_MOD and copies to r1.
*/
int inst_ldi(champion_t *, core_t *, code_t, int *);

/* 
Takes 3 parameters. The first one is a register. The other two can either
be indexes or registers. Ex: sti r2, %4, %5 -> copies the content for r2
into Counter + (4 + 5) % IDX_MOD.
*/
int inst_sti(champion_t *, core_t *, code_t, int *);

/* 
Takes 1 parameter, an index. Creates a new program, inheriting states
from the parent, and starting execution at COunter + parameter %
IDX_MOD
*/
int inst_fork(champion_t *, core_t *, code_t, int *);

/* 
Same as ld without the %IDX_MOD. Modifies the carry
*/
int inst_lld(champion_t *, core_t *, code_t, int *);

/* 
Same as ldi without the %IDX_MOD. Modifies the carry
*/
int inst_lldi(champion_t *, core_t *, code_t, int *);

/* 
Same as fork without the %IDX_MOD.
*/
int inst_lfork(champion_t *, core_t *, code_t, int *);

/* 
Takes 1 parameter, a register. Displays to stdout the character
corresponding to the ASCII code of the content of the register (in base
10). A 256 modulo is applied to this ASCII code
*/
int inst_aff(champion_t *, core_t *, code_t, int *);


void print_colored_text(int color);

void display_memory(const core_t *core);

#endif