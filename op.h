#ifndef _OP_H_
#define _OP_H_
#define MEM_SIZE (6 * 1024)
/* modulo of the index < */
#define IDX_MOD 512
/* this may not be changed 2^*IND_SIZE */
#define MAX_ARGS_NUMBER 4
#define COMMENT_CHAR \'#'
#define LABEL_CHAR ':'
#define DIRECT_CHAR '%'
#define SEPARATOR_CHAR ','
#define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"
#define NAME_CMD_STRING ".name"
#define COMMENT_CMD_STRING ".comment"
/*r1<-->rx*/
#define REG_NUMBER 16
typedef char args_type_t;
typedef unsigned char code_t;
enum parameter_types { T_REG = 1, T_DIR = 2, T_IND = 4, T_LAB = 8 };

typedef struct champion champion_t;
typedef struct core_s core_t;
struct op_s {
  char *mnemonique;
  char nbr_args;
  args_type_t type[MAX_ARGS_NUMBER];
  char code;
  int nbr_cycles;
  int (*inst)(champion_t *, core_t *, code_t, int *);
};

enum op_types {
  OP_LIVE,
  OP_LD,
  OP_ST,
  OP_ADD,
  OP_SUB,
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_ZJMP,
  OP_LDI,
  OP_STI,
  OP_FORK,
  OP_LLD,
  OP_LLDI,
  OP_LFORK,
  OP_AFF,
  OP_NOTHING,
  OP_NB
};
typedef struct op_s op_t;
/* size (in bytes) */
#define IND_SIZE 2
#define DIR_SIZE 4
#define REG_SIZE DIR_SIZE
/* op_tab */
extern const op_t op_tab[];
/* HEADER */
#define PROG_NAME_LENGTH 128
#define COMMENT_LENGTH 2048
#define COREWAR_EXEC_MAGIC 0xea83f3
typedef struct header_s {
  int magic;
  char prog_name[PROG_NAME_LENGTH + 1];
  int prog_size;
  char comment[COMMENT_LENGTH + 1];
} header_t; /* live */

#define MAX_CHAMPIONS 4
/* number of cycle before being declared dead */ 
#define CYCLE_TO_DIE 1536
#define CYCLE_DELTA 5
#define NBR_LIVE 40

// Implement me

/* 
Takes 1 parameter: 4 bytes that represent the player's number.
Indicates the player is alive.
*/
void inst_live();

/* 
Takes 2 parameters: loads the first parameter to the second parameter.
Second parameter is a register. Ex: ld 34, r3 loads the REG_SIZE bytes
starting from the Counter + 34 % IDX_MOD into r3.
*/
void inst_ld();

/* 
Takes 2 parameters: Stores first parameter (a register) into the second
parameter (it can be another register or a number). Ex: st r4, 34 stores
r4 at the address of Counter + 34 % IDX_MOD; st r3, r8 copies r3 into
r8.
*/
void inst_st();

/* 
Takes 3 parameters: 3 registers. Add the first to the second, and store
the result to the third. Modifies the carry.
*/
void inst_add();

/* 
Same as add, but Subtracting. Modifies the carry.
*/
void inst_sub();

/* 
Same as add, and sub, but does a binary operation AND between the
first and the second, and storing the result in the third parameter.
Modifies the carry.
*/
void inst_and();

/* 
Same as and, but performing an OR.
*/
void inst_or();

/* 
Same as and and or, but performing an XOR.
*/
void inst_xor();

/* 
Takes 1 parameter, that must be an index. Jumps to this index if carry is
1. Else, does nothing, but still consume time.
Zjmp %23 -> if carry == 1, Counter + 23 % IDX_MOD to Counter
*/
void inst_zjmp();

/* 
Takes 3 parameters. First two are indexes and the third one is a register
Ex: ldi 3, %4, r1 -> reads IND_SIZ bytes from address Counter + 3 %
IDX_MOD, add 4 to this value (SumResult). REG_SIZE byte are read
from the Counter + SumResult % IDX_MOD and copies to r1.
*/
void inst_ldi();

/* 
Takes 3 parameters. The first one is a register. The other two can either
be indexes or registers. Ex: sti r2, %4, %5 -> copies the content for r2
into Counter + (4 + 5) % IDX_MOD.
*/
void inst_sti();

/* 
Takes 1 parameter, an index. Creates a new program, inheriting states
from the parent, and starting execution at COunter + parameter %
IDX_MOD
*/
void inst_fork();

/* 
Same as ld without the %IDX_MOD. Modifies the carry
*/
void inst_lld();

/* 
Same as ldi without the %IDX_MOD. Modifies the carry
*/
void inst_lldi();

/* 
Same as fork without the %IDX_MOD.
*/
void inst_lfork();

/* 
Takes 1 parameter, a register. Displays to stdout the character
corresponding to the ASCII code of the content of the register (in base
10). A 256 modulo is applied to this ASCII code
*/
void inst_aff();

#endif