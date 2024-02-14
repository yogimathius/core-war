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


// Now update the encode_instruction function to use these
void encode_instruction(FILE *output, ParsedLine *parsedLine) {
    enum op_types optype = (enum op_types)strtoul(parsedLine->opcode, NULL, 16);

    op_t operation = op_tab[optype];
    printf("opcode: %d, mnemonic: %s, args: %d\n", operation.code, operation.mnemonique, operation.nbr_args);

    char opcode = operation.code;

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
        printf("data: %s %s %i\n", parsedLine.opcode, parsedLine.label, parsedLine.lineType);
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
            encode_instruction(output, &parsedLine);
            // Increment current_address by the size of the instruction
            current_address += 4; // Placeholder, replace with actual instruction size
        }
        // Handle other line types as needed
    }
}

int main(int argc, const char *argv[]) {
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