#include "./op.h"

// Ensure that registers are encoded as a single byte
void encode_register(FILE *output, const char *arg, int *current_address) {
    unsigned char reg_num = (unsigned char)atoi(arg + 1); // Skip 'r' and convert to int
    fwrite(&reg_num, REG_SIZE, 1, output); // Write a single byte
    (*current_address) += 1;
}

void encode_indirect(FILE *output, const char *arg, int *current_address) {
    int direct_value = atoi(arg);
    printf("direct_value: %d\n", direct_value);
    unsigned int big_endian_value = htonl(direct_value);
    printf("big_endian_value: %u\n", big_endian_value);
    fwrite(&big_endian_value, IND_SIZE, 1, output);
    (*current_address) += 2;
}

int containsDigits(const char *str) {
    while (*str != '\0') {
        if (isdigit(*str)) {
            return 1; 
        }
        str++; 
    }
    return 0;
}

void encode_direct(FILE *output, const char *arg, int *current_address) {
    int indirect_value;
    if (arg[0] == '%' && containsDigits(arg + 1) == 0) {
        printf("label: %s\n", arg + 2);
        // Handle as label
        int label_address = lookup_symbol(arg + 2); // Skip '%' and lookup label
        indirect_value = label_address - *current_address;
        printf("indirect_value: %d\n", indirect_value);
    } else if (arg[0] == '%' && containsDigits(arg + 1) == 1) {
        indirect_value = atoi(arg + 1);
        if (indirect_value == 0) {
            return encode_indirect(output, arg, current_address);
        }
    } else {
        printf("arg in direct: %s\n", arg);
        // Handle as numeric offset
        indirect_value = atoi(arg);
    }
    
    unsigned int big_endian_value = htonl(indirect_value);

    fwrite(&big_endian_value, DIR_SIZE, 1, output);
    *current_address += 4;
}


unsigned char encode_parameter_description(const char arguments[][MAX_ARGUMENT_LENGTH], int argumentCount) {
    unsigned char description = 0;
    for (int i = 0; i < argumentCount; i++) {
        unsigned char paramCode = 0;
        if (arguments[i][0] == 'r') {
            paramCode = REG_CODE; // 01 in binary for register
        } else if (isdigit(arguments[i][0]) || (arguments[i][0] == '-' && isdigit(arguments[i][1]))) {
            paramCode = IND_CODE; // 11 in binary for indirect
        } else {
            paramCode = DIR_CODE; // 10 in binary for direct
        }
        description |= paramCode << (6 - 2 * i);
    }
    return description;
}

enum op_types mnemonic_to_op_type(const char *mnemonic) {
    for (int i = 0; i < OP_TYPES_COUNT; i++) {
        if (strcmp(mnemonic, op_tab[i].mnemonique) == 0) {
            return (enum op_types)i;
        }
    }
    return OP_NOTHING;
}

void encode_arguments(FILE *output, const parsed_line_t *parsedLine, int *current_address) {
    for (int i = 0; i < parsedLine->argumentCount; i++) {
        if (parsedLine->arguments[i][0] == 'r') {
            encode_register(output, parsedLine->arguments[i], current_address);
        } else if (parsedLine->arguments[i][0] == '%') {
            encode_direct(output, parsedLine->arguments[i], current_address);
        } else {
            encode_indirect(output, parsedLine->arguments[i], current_address);
        }
    }
}

int is_one_byte_instruction(const char *opcode) {
    return (
        strcmp(opcode, "live") == 0 || strcmp(opcode, "zjmp") == 0 || strcmp(opcode, "fork") == 0 || strcmp(opcode, "lfork") == 0
    );
}

void log_instruction(const parsed_line_t *parsedLine) {
    printf("\n===================ENCODING INSTRUCTION==================\n\n");
    printf("Encoding instruction: %s with ", parsedLine->opcode);
    printf("Arguments: %d ", parsedLine->argumentCount);
    for (int i = 0; i < parsedLine->argumentCount; i++) {
        printf("%s ", parsedLine->arguments[i]);
    }
    printf("\n");
}

void encode_instruction(FILE *output, parsed_line_t *parsedLine, int *current_address) {
    log_instruction(parsedLine);
    enum op_types op_type = mnemonic_to_op_type(parsedLine->opcode);

    op_t operation = op_tab[op_type];

    unsigned char opcode = operation.code;
    // Write the opcode to the output file.
    fwrite(&opcode, sizeof(opcode), 1, output);
    (*current_address) += 1;

    // Then write the parameter description byte
    unsigned char param_description = encode_parameter_description(parsedLine->arguments, parsedLine->argumentCount);
    (*current_address) += 1;
    fwrite(&param_description, sizeof(param_description), 1, output);

    // Then write the arguments
    if (
        is_one_byte_instruction(parsedLine->opcode)
    ) {
        printf("1 arg 4 bytes: %s\n", parsedLine->arguments[0]);
        if (parsedLine->arguments[0][1] == ':') {
            encode_direct(output, parsedLine->arguments[0], current_address);
        } else {
            encode_indirect(output, parsedLine->arguments[0], current_address);
        }
    } else {
        encode_arguments(output, parsedLine, current_address);
        
    }
}

void write_magic_number(FILE *output) {
    int corewar_exec_magic = COREWAR_EXEC_MAGIC;
    unsigned char magic_number[4];

    magic_number[0] = (corewar_exec_magic >> 24) & 0xFF;
    magic_number[1] = (corewar_exec_magic >> 16) & 0xFF;
    magic_number[2] = (corewar_exec_magic >> 8) & 0xFF;
    magic_number[3] = corewar_exec_magic & 0xFF;

    fwrite(magic_number, sizeof(int), 1, output);
}

void write_header(FILE *output, FileHeader *header) {
    write_magic_number(output);

    fwrite(&header->name, sizeof(char), sizeof(char) * PROG_NAME_LENGTH, output);
    fwrite("\0\0\0\0", sizeof(char), 4, output); // Placeholder for program instructions size (4 bytes)

    unsigned int size = htonl(header->size);
    fwrite(&size, sizeof(int), 1, output);

    fwrite(&header->comment, sizeof(char), sizeof(char) * COMMENT_LENGTH, output);
    fwrite("\0\0\0\0", sizeof(char), 4, output); // Placeholder for program instructions size (4 bytes)
}