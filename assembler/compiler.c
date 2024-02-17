#include "./op.h"

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
            // printf("Encoding instruction: %s\n", parsedLine.opcode);
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

    FileHeader header = parse_header(input_file);

    // Open output file and write header
    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    int corewar_exec_magic = COREWAR_EXEC_MAGIC;

    // Hacky way to write the magic number to the output file in big-endian format
    unsigned char bytes[4];
    bytes[0] = (corewar_exec_magic >> 24) & 0xFF;
    bytes[1] = (corewar_exec_magic >> 16) & 0xFF;
    bytes[2] = (corewar_exec_magic >> 8) & 0xFF;
    bytes[3] = corewar_exec_magic & 0xFF;

    fwrite(bytes, sizeof(int), 1, output_file);
    fwrite(header.name, sizeof(char), PROG_NAME_LENGTH, output_file);

    fwrite(header.comment, sizeof(char), COMMENT_LENGTH, output_file);

    assemble(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}