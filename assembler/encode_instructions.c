#include "./op.h"

// Ensure that registers are encoded as a single byte
void encode_register(FILE *output, const char *arg) {
    unsigned char reg_num = (unsigned char)atoi(arg + 1); // Skip 'r' and convert to int
    fwrite(&reg_num, T_REG, 1, output); // Write a single byte
}


void encode_direct(FILE *output, const char *arg) {
    int direct_value = atoi(arg);
    fwrite(&direct_value, T_DIR, 1, output); // This will write in little-endian format
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
    fwrite(&indirect_value, T_IND, 1, output); // This will write in little-endian format
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

enum op_types mnemonic_to_op_type(const char *mnemonic) {
    for (int i = 0; i < OP_TYPES_COUNT; i++) {
        if (strcmp(mnemonic, op_tab[i].mnemonique) == 0) {
            return (enum op_types)i;
        }
    }
    return OP_NOTHING;
}
// Now update the encode_instruction function to use these
void encode_instruction(FILE *output, ParsedLine *parsedLine) {
    printf("Encoding instruction: %s\n", parsedLine->opcode);
    enum op_types op_type = mnemonic_to_op_type(parsedLine->opcode);

    printf("optype: %d\n", op_type);
    op_t operation = op_tab[op_type];
    printf("opcode: %d, mnemonic: %s, args: %d\n", operation.code, operation.mnemonique, operation.nbr_args);

    unsigned char opcode = operation.code;
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
            encode_register(output, arg);
        } else if (*arg == '%') {
            // Write indirect (2 bytes)
            encode_indirect(output, arg, 0); // Replace 0 with the current address
        } else if (isdigit(*arg) || (*arg == '-' && isdigit(*(arg + 1)))) {
            // Write direct (4 bytes)
            encode_direct(output, arg);
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