#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#ifndef _CHAMPION_H_
#define _CHAMPION_H_
#define COREWAR_EXEC_MAGIC 0xea83f3

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

#define MAX_ARGS_NUMBER 3
#define REG_CODE 1
#define DIR_CODE 2
#define IND_CODE 3

#define REG_SIZE 1
#define DIR_SIZE 2
#define IND_SIZE 4

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
  OP_NB, // 0X12
  OP_TYPES_COUNT
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


// Enumeration for different types of tokens
typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_INSTRUCTION,
    TOKEN_SEPARATOR,
    TOKEN_LABEL,
    TOKEN_DIRECT,
    TOKEN_COMMENT,
    TOKEN_REGISTER,
    TOKEN_INDIRECT,
    TOKEN_OPERATOR,
    TOKEN_BLANKLINE
} TokenType;


// Constants based on your assignment details
#define MAX_LINE_LENGTH 256
#define MAX_LABEL_LENGTH 50
#define MAX_ARGUMENT_LENGTH 50
#define PROG_NAME_LENGTH 128
#define COMMENT_LENGTH 2048
#define MAX_PROG_LENGTH (6 * 1024) / 4

// Structure to hold a token
typedef struct {
    TokenType type;
    char string[MAX_LABEL_LENGTH];
} Token;

// Structure to hold a parsed line
typedef struct {
    TokenType lineType;
    char label[MAX_LABEL_LENGTH];
    char opcode[MAX_LABEL_LENGTH];
    char arguments[MAX_ARGS_NUMBER][MAX_ARGUMENT_LENGTH];
    int argumentCount;
} parsed_line_t;

typedef struct Symbol {
    char label[MAX_LABEL_LENGTH];
    int address;
    struct Symbol *next;
} Symbol;

typedef struct {
    char name[PROG_NAME_LENGTH + 2];
    char comment[COMMENT_LENGTH + 1];
    int size;
    int parsed_lines_size;
    parsed_line_t parsed_lines[MAX_PROG_LENGTH];
} FileHeader;

// Function prototypes
Token lex_token(const char **input);
void add_symbol(const char *label, int address);
int lookup_symbol(const char *label);
void encode_instruction(FILE *output, parsed_line_t *parsedLine, int *current_address);
void assemble(FILE *output, FileHeader *header);
void write_little_endian(FILE *output, int value);
void parse_contents(FILE *input, FileHeader *header);
void encode_header(FILE *output, FileHeader *header);
void encode_magic_number(FILE *output);
FileHeader *init_header();

#endif