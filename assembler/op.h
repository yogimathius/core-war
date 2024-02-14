#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#ifndef _CHAMPION_H_
#define _CHAMPION_H_

// Character denoting comments in the code
#define COMMENT_CHAR '#'

// Character denoting labels in the assembly code
#define LABEL_CHAR ':'

// Character denoting direct addressing mode in assembly code
#define DIRECT_CHAR '%'

// Character used to separate arguments in assembly code
#define SEPARATOR_CHAR ','

// Characters allowed in labels
#define LABEL_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"

// Directive string for the program name
#define NAME_CMD_STRING ".name"

// Directive string for the comment
#define COMMENT_CMD_STRING ".comment"

#define MAX_ARGS_NUMBER 4

enum parameter_types { T_REG = 1, T_DIR = 2, T_IND = 4, T_LAB = 8 };

typedef char args_type_t;

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

typedef struct op_s {
  char *mnemonique;     // Operation mnemonic
  char nbr_args;        // Number of arguments
  args_type_t type[MAX_ARGS_NUMBER];  // Types of arguments
  char code;            // Operation code
} t_op;

typedef struct op_s op_t;

// Array of operation structures

extern const op_t op_tab[];

#endif