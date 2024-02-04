#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

// Constants based on your assignment details
#define MAX_LINE_LENGTH 256
#define MAX_LABEL_LENGTH 50
#define MAX_ARGUMENTS 3
#define MAX_ARGUMENT_LENGTH 50
#define REGISTER_SIZE 1
#define DIRECT_SIZE 4 // Assuming 'direct' arguments are 4 bytes
#define INDIRECT_SIZE 4 // Assuming 'indirect' arguments are 4 bytes
#define OPCODE_LIVE 0x01
#define OPCODE_LD 0x02
#define OPCODE_ST 0x03
#define OPCODE_ADD 0x04
#define OPCODE_SUB 0x05
#define OPCODE_AND 0x06
#define OPCODE_OR 0x07
#define OPCODE_XOR 0x08
#define OPCODE_ZJMP 0x09
#define OPCODE_LDI 0x0a
#define OPCODE_STI 0x0b
#define OPCODE_FORK 0x0c
#define OPCODE_LLD 0x0d
#define OPCODE_LLDI 0x0e
#define OPCODE_LFORK 0x0f
#define OPCODE_AFF 0x10

// Enumeration for different types of tokens
typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_INSTRUCTION,
    TOKEN_SEPARATOR,
    TOKEN_LABEL,
    TOKEN_DIRECTIVE,
    TOKEN_COMMENT,
    TOKEN_REGISTER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_ENDLINE
} TokenType;

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
    char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
    int argumentCount;
} ParsedLine;


typedef struct Symbol {
    char label[MAX_LABEL_LENGTH];
    int address;
    struct Symbol *next;
} Symbol;

// Function prototypes
Token lex_token(const char **input);
ParsedLine parse_line(const char *line);
void add_symbol(const char *label, int address);
int lookup_symbol(const char *label);
void encode_instruction(FILE *output, ParsedLine *parsedLine, int current_address);
void assemble(FILE *input, FILE *output);


Symbol *symbol_table = NULL;

// Function to add a symbol to the table
void add_symbol(const char *label, int address) {
    Symbol *symbol = malloc(sizeof(Symbol));
    strcpy(symbol->label, label);
    symbol->address = address;
    symbol->next = symbol_table;
    symbol_table = symbol;
    printf("Added symbol: %s with address: %d\n", label, address); // Debug print

}

// Function to look up a symbol in the table
int lookup_symbol(const char *label) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        if (strcmp(current->label, label) == 0) {
            printf("Found symbol: %s at address: %d\n", label, current->address); // Debug print
            return current->address;
        }
        current = current->next;
    }
    return -1; // Not found
}

Token lex_token(const char **input) {
    Token token = {TOKEN_UNKNOWN, ""};
    const char *start = *input;

    // Skip whitespace
    while (isspace((unsigned char)**input)) {
        (*input)++;
    }

    if (**input == '\0') {
        token.type = TOKEN_ENDLINE;
    } else if (**input == ';') { // Assuming ';' starts a comment
        token.type = TOKEN_COMMENT;
        while (**input && **input != '\n') {
            (*input)++;
        }
    } else if (**input == '.') { // Directive
        token.type = TOKEN_DIRECTIVE;
        (*input)++;
        start = *input; // Start after the '.'
        while (!isspace((unsigned char)**input) && **input != '\0') {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';

        // Skip whitespace after directive keyword
        while (isspace((unsigned char)**input)) {
            (*input)++;
        }

        // Store the rest of the line as an argument (if present)
        start = *input;
        while (**input != '\n' && **input != '\0') {
            (*input)++;
        }
        if (start != *input) { // There's additional text after the directive keyword
            length = *input - start;
            if (length > MAX_ARGUMENT_LENGTH - 1) {
                length = MAX_ARGUMENT_LENGTH - 1; // Limit the length to avoid buffer overflow
            }
            strncpy(token.string, start, length);
            token.string[length] = '\0';
        }
    } else if (isalpha((unsigned char)**input)) { // Label or Instruction
        start = *input;
        while (isalnum((unsigned char)**input) || **input == '_') {
            (*input)++;
        }
        if (**input == ':') { // It's a label
            token.type = TOKEN_LABEL;
            (*input)++;
        } else { // It's an instruction
            token.type = TOKEN_INSTRUCTION;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (isdigit((unsigned char)**input) || **input == '-') { // Number
        token.type = TOKEN_NUMBER;
        if (**input == '-') {
            (*input)++;
        }
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (**input == 'r' && isdigit((unsigned char)*(*input + 1))) { // Register
        token.type = TOKEN_REGISTER;
        (*input)++; // Skip 'r'
        start = *input; // Start at the digit
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (**input == '%' && *(*input + 1) == ':') { // Direct label
        token.type = TOKEN_DIRECTIVE;
        (*input) += 2; // Skip '%:'
        start = *input; // Start after '%:'
        while (isalnum((unsigned char)**input) || **input == '_') {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }
    else if (**input == '%' && isdigit((unsigned char)*(*input + 1))) { // Direct value
        token.type = TOKEN_NUMBER; // or a new type, e.g., TOKEN_DIRECT_VALUE
        (*input)++; // Skip '%'
        start = *input; // Start at the digit
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }   
    else if (**input == ',') {  // Comma separator
        token.type = TOKEN_SEPARATOR;
        (*input)++;  // Skip the comma
    }
    else if (**input == 'r' && isdigit((unsigned char)*(*input + 1))) { // Register
        token.type = TOKEN_REGISTER;
        (*input)++; // Skip 'r'
        start = *input; // Start at the digit
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }

    printf("input: %s\n", *input);

    return token;
}

ParsedLine parse_line(const char *line) {
    ParsedLine parsedLine = {TOKEN_UNKNOWN, "", "", {""}, 0};
    const char *inputPtr = line;

    Token token;

    // Check for empty line or comment
    token = lex_token(&inputPtr);
    printf("token type: %i\n", token.type);
    printf("token string: %s\n", token.string);
    if (token.type == TOKEN_ENDLINE || token.type == TOKEN_COMMENT) {
        parsedLine.lineType = token.type;
        return parsedLine;
    }

    // Process the first token which could be a label or an instruction
    if (token.type == TOKEN_LABEL) {
        strcpy(parsedLine.label, token.string);
        token = lex_token(&inputPtr); // Get next token after label
    }

    if (token.type == TOKEN_INSTRUCTION) {
        parsedLine.lineType = TOKEN_INSTRUCTION;
        strcpy(parsedLine.opcode, token.string);
    } else if (token.type == TOKEN_DIRECTIVE) {
        parsedLine.lineType = TOKEN_DIRECTIVE;
        strcpy(parsedLine.opcode, token.string);
    }
    printf("parsedLine.opcode: %s\n", parsedLine.opcode);

    // Process arguments if any
    while ((token = lex_token(&inputPtr)).type != TOKEN_ENDLINE && token.type != TOKEN_COMMENT) {
        printf("token type: %i\n", token.type);
        if (parsedLine.argumentCount < MAX_ARGUMENTS) {
            strcpy(parsedLine.arguments[parsedLine.argumentCount++], token.string);
        } else {
            // Handle error: too many arguments
        }
    }
    printf("parsedLine.argumentCount: %i\n", parsedLine.argumentCount);
    fflush(stdout);

    return parsedLine;
}

// Ensure that registers are encoded as a single byte
void encode_register(FILE *output, const char *arg) {
    unsigned char reg_num = (unsigned char)atoi(arg + 1); // Skip 'r' and convert to int
    fwrite(&reg_num, 1, 1, output); // Write a single byte
}


void encode_direct(FILE *output, const char *arg) {
    int direct_value = atoi(arg);
    fwrite(&direct_value, DIRECT_SIZE, 1, output); // This will write in little-endian format
}

void encode_indirect(FILE *output, const char *arg, int current_address) {
    int indirect_value;
    if (arg[0] == '%') {
        // Handle as label
        int label_address = lookup_symbol(arg + 1); // Skip '%' and lookup label
        indirect_value = label_address - current_address;
    } else {
        // Handle as numeric offset
        indirect_value = atoi(arg);
    }
    fwrite(&indirect_value, INDIRECT_SIZE, 1, output); // This will write in little-endian format
}


unsigned char encode_parameter_description(char arguments[][MAX_ARGUMENT_LENGTH], int argumentCount) {
    unsigned char description = 0;
    // The bits are set from left to right for each parameter.
    for (int i = 0; i < argumentCount; i++) {
        unsigned char paramCode = 0;
        if (arguments[i][0] == 'r') {
            paramCode = 1; // 01 in binary for register
        } else if (isdigit(arguments[i][0]) || (arguments[i][0] == '-' && isdigit(arguments[i][1]))) {
            paramCode = 2; // 10 in binary for direct
        } else {
            paramCode = 3; // 11 in binary for indirect
        }
        description |= paramCode << (6 - 2 * i);
    }
    return description;
}

void write_little_endian(FILE *output, int value) {
    unsigned char bytes[4];
    bytes[0] = (value >> 0) & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
    fwrite(bytes, sizeof(bytes), 1, output);
}


// Now update the encode_instruction function to use these
void encode_instruction(FILE *output, ParsedLine *parsedLine, int current_address) {
    unsigned char opcode;
    if (strcmp(parsedLine->opcode, "live") == 0) {
        opcode = OPCODE_LIVE;
    } else if (strcmp(parsedLine->opcode, "ld") == 0) {
        opcode = OPCODE_LD;
    } else if (strcmp(parsedLine->opcode, "st") == 0) {
        opcode = OPCODE_ST;
    } else if (strcmp(parsedLine->opcode, "add") == 0) {
        opcode = OPCODE_ADD;
    } else if (strcmp(parsedLine->opcode, "sub") == 0) {
        opcode = OPCODE_SUB;
    } else if (strcmp(parsedLine->opcode, "and") == 0) {
        opcode = OPCODE_AND;
    } else if (strcmp(parsedLine->opcode, "or") == 0) {
        opcode = OPCODE_OR;
    } else if (strcmp(parsedLine->opcode, "xor") == 0) {
        opcode = OPCODE_XOR;
    } else if (strcmp(parsedLine->opcode, "zjmp") == 0) {
        opcode = OPCODE_ZJMP;
    } else if (strcmp(parsedLine->opcode, "ldi") == 0) {
        opcode = OPCODE_LDI;
    } else if (strcmp(parsedLine->opcode, "sti") == 0) {
        opcode = OPCODE_STI;
    } else if (strcmp(parsedLine->opcode, "fork") == 0) {
        opcode = OPCODE_FORK;
    } else if (strcmp(parsedLine->opcode, "lld") == 0) {
        opcode = OPCODE_LLD;
    } else if (strcmp(parsedLine->opcode, "lldi") == 0) {
        opcode = OPCODE_LLDI;
    } else if (strcmp(parsedLine->opcode, "lfork") == 0) {
        opcode = OPCODE_LFORK;
    } else if (strcmp(parsedLine->opcode, "aff") == 0) {
        opcode = OPCODE_AFF;
    } 

    // Write the opcode to the output file.
    fwrite(&opcode, sizeof(opcode), 1, output);

    // Then write the parameter description byte
    unsigned char param_description = encode_parameter_description(parsedLine->arguments, parsedLine->argumentCount);
    fwrite(&param_description, sizeof(param_description), 1, output);

    // Then write the arguments
    for (int i = 0; i < parsedLine->argumentCount; i++) {
        char* arg = parsedLine->arguments[i];

        // Trim leading whitespace
        while (isspace((unsigned char)*arg)) { arg++; }

        if (*arg == 'r') {
            // Write register (1 byte)
            unsigned char reg = (unsigned char)atoi(arg + 1);
            fwrite(&reg, sizeof(reg), 1, output);
        } else {
            // Write direct/indirect (4 bytes)
            int value = 0;
            if (isdigit(*arg) || (*arg == '-' && isdigit(*(arg + 1)))) {
                // Direct value, just convert to integer
                value = atoi(arg);
                write_little_endian(output, value);
            } else {
                // Indirect value, must be a label, so look it up
                value = lookup_symbol(arg);
                if (value == -1) {
                    fprintf(stderr, "Error: Symbol '%s' not found.\n", arg);
                    // Handle error accordingly, but do not exit.
                    value = 0; // Placeholder for error handling.
                }
                write_little_endian(output, value);
            }
        }
    }
}

// Main assembler loop with two passes
void assemble(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    int current_address = 0; // Start address for the code
    // printf("%i\n", current_address);
    // fflush(stdout);

    // First pass: Build the symbol table
    while (fgets(line, sizeof(line), input)) {
        printf("Processing line: %s\n", line); 
        ParsedLine parsedLine = parse_line(line);
        printf("data: %s %s %i", parsedLine.opcode, parsedLine.label, parsedLine.lineType);
        // printf("%s %s", parsedLine.opcode, parsedLine.label);
        // fflush(stdout);
        if (parsedLine.lineType == TOKEN_LABEL) {
            add_symbol(parsedLine.label, current_address);
        } else if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            // Here you'd calculate the size of the instruction and increment current_address
            // For now, we'll just increment it by a placeholder value
            current_address += 4; // Placeholder, replace with actual instruction size
        }
    }

    // Prepare for second pass
    fseek(input, 0, SEEK_SET); // Reset file pointer to beginning of input file
    current_address = 0; // Reset address for encoding

    // Second pass: Encode instructions
    while (fgets(line, sizeof(line), input)) {
        ParsedLine parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            encode_instruction(output, &parsedLine, current_address);
            // Increment current_address by the size of the instruction
            current_address += 4; // Placeholder, replace with actual instruction size
        }
        // Handle other line types as needed
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file.asm> <output_file.cor>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file); // Make sure to close the input file before exiting
        return EXIT_FAILURE;
    }
    // printf("input file: %s\n", argv[1]);
    // fflush(stdout);

    assemble(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}