#include "./op.h"

typedef struct {
    char name[MAX_LABEL_LENGTH];
    char comment[MAX_LINE_LENGTH];
} FileHeader;

FileHeader parse_header(FILE *input) {
    FileHeader header = {"", ""};
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), input)) {
        if (strncmp(line, ".name", 5) == 0) {
            sscanf(line, ".name \"%[^\"]\"", header.name);
        } else if (strncmp(line, ".comment", 8) == 0) {
            sscanf(line, ".comment \"%[^\"]\"", header.comment);
        } else {
            break;
        }
    }

    rewind(input);

    return header;
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
    fwrite(&header, sizeof(FileHeader), 1, output_file);
    // printf("input file: %s\n", argv[1]);
    // fflush(stdout);

    assemble(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}