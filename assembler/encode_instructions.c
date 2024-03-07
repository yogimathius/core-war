#include "./op.h"

// Ensure that registers are encoded as a single byte
void encode_register(FILE *output, const char *arg) {
    unsigned char reg_num = (unsigned char)atoi(arg + 1); // Skip 'r' and convert to int
    fwrite(&reg_num, REG_SIZE, 1, output); // Write a single byte
}


void encode_indirect(FILE *output, const char *arg) {
    int direct_value = atoi(arg);
    printf("direct_value: %d\n", direct_value);
    unsigned int big_endian_value = htonl(direct_value);
    printf("big_endian_value: %u\n", big_endian_value);
    fwrite(&big_endian_value, IND_SIZE, 1, output);
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

void encode_direct(FILE *output, const char *arg, int current_address) {
    int indirect_value;
    if (arg[0] == '%' && containsDigits(arg + 1) == 0) {
        printf("label: %s\n", arg);
        // Handle as label
        int label_address = lookup_symbol(arg + 2); // Skip '%' and lookup label
        printf("label_address: %d\n", label_address);
        indirect_value = label_address - current_address;
    } else if (arg[0] == '%' && containsDigits(arg + 1) == 1) {
        indirect_value = atoi(arg + 1); // Skip '%' and convert to int
    } else {
        printf("arg in direct: %s\n", arg);
        // Handle as numeric offset
        indirect_value = atoi(arg);
    }
    
    unsigned int big_endian_value = htonl(indirect_value);

    fwrite(&big_endian_value, DIR_SIZE, 1, output);
}


unsigned char encode_parameter_description(char arguments[][MAX_ARGUMENT_LENGTH], int argumentCount) {
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
// Now update the encode_instruction function to use these
void encode_instruction(FILE *output, parsed_line_t *parsedLine) {
    printf("Encoding instruction: %s with ", parsedLine->opcode);
    printf("Arguments: %d ", parsedLine->argumentCount);
    for (int i = 0; i < parsedLine->argumentCount; i++) {
        printf("%s ", parsedLine->arguments[i]);
    }
    printf("\n");
    enum op_types op_type = mnemonic_to_op_type(parsedLine->opcode);

    op_t operation = op_tab[op_type];

    unsigned char opcode = operation.code;
    // Write the opcode to the output file.
    fwrite(&opcode, sizeof(opcode), 1, output);

    // printf("\n===================ENCODING PD==================\n");
    // Then write the parameter description byte
    unsigned char param_description = encode_parameter_description(parsedLine->arguments, parsedLine->argumentCount);
    fwrite(&param_description, sizeof(param_description), 1, output);

    // Then write the arguments
    printf("\n===================ENCODING ARGS==================\n");
    for (int i = 0; i < parsedLine->argumentCount; i++) {
        char* arg = parsedLine->arguments[i];

        // Trim leading whitespace
        while (isspace((unsigned char)*arg)) { arg++; }
        printf("arg in encode: %s\n", arg);
        if (*arg == 'r') {
            printf("register: %s\n", arg);
            encode_register(output, arg);
        } else if (*arg == '%') {
            printf("direct: %s\n", arg);
            // (*arg)++;
            // if (*arg == LABEL_CHAR) {
            //     printf("label: %s\n", arg);
            //     (*arg)++;
            // }
            
            encode_direct(output, arg, 0); 
        } else if (isdigit(*arg) || (*arg == '-' && isdigit(*(arg + 1)))) {
            // Write direct (4 bytes)
            printf("indirect: %s\n", arg);
            encode_indirect(output, arg);
        } else {
            // Handle error accordingly, but do not exit.
            fprintf(stderr, "Error: Invalid argument '%s'.\n", arg);
        }
    }
}