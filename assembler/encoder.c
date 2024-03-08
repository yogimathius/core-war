#include "./op.h"

void write_big_endian(FILE *output, int value) {
    uint8_t bytes[sizeof(int)];
    for (int i = sizeof(int) - 1; i >= 0; i--) {
        bytes[i] = (uint8_t)(value & 0xFF);
        value >>= 8;
    }

    fwrite(bytes, sizeof(int), 1, output);
}

void write_last_two_bytes(FILE *output, int value) {
    uint8_t byte3 = (value >> 8) & 0xFF; 
    uint8_t byte4 = value & 0xFF;

    fwrite(&byte3, sizeof(uint8_t), 1, output);
    fwrite(&byte4, sizeof(uint8_t), 1, output);
}

void encode_register(FILE *output, const char *arg, int *current_address) {
    unsigned char reg_num = (unsigned char)atoi(arg + 1); // Skip 'r' and convert to int
    fwrite(&reg_num, REG_SIZE, 1, output); // Write a single byte
    (*current_address) += 1;
}

void encode_indirect(FILE *output, const char *arg, int *current_address) {
    int direct_value = atoi(arg);
    write_big_endian(output, direct_value);
    (*current_address) += 4;
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
    printf("arg in encode direct: %s\n", arg);

    int indirect_value;
    if (arg[0] == '%' && containsDigits(arg + 1) == 0) {
        int label_address = lookup_symbol(arg + 2); // Skip '%' and lookup label
        indirect_value = label_address - *current_address;
        write_last_two_bytes(output, indirect_value);
    } else if (arg[0] == '%' && containsDigits(arg + 1) == 1) {
        indirect_value = atoi(arg + 1);
        if (indirect_value == 0) {
            return encode_indirect(output, arg, current_address);
        }

        write_last_two_bytes(output, indirect_value);
    } else {
        indirect_value = atoi(arg);
        fwrite(&indirect_value, DIR_SIZE, 1, output);

    }

    *current_address += 2;
}


unsigned char calc_parameter_description(const char arguments[][MAX_ARGUMENT_LENGTH], int argumentCount) {
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

void log_instruction(const parsed_line_t *parsedLine) {
    printf("\n===================ENCODING INSTRUCTION==================\n\n");
    printf("Encoding instruction: %s with ", parsedLine->opcode);
    printf("Arguments: ");
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
    fwrite(&opcode, sizeof(opcode), 1, output);
    if (*current_address != 0) {
        (*current_address) += 1;
    }

    if (parsedLine->argumentCount > 1) {
        unsigned char param_description = calc_parameter_description(parsedLine->arguments, parsedLine->argumentCount);
        fwrite(&param_description, sizeof(param_description), 1, output);
        (*current_address) += 1;
    }

    if (
        parsedLine->argumentCount == 1
    ) {
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