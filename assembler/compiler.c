#include "./op.h"

void assemble(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    int current_address = 0; 

    while (fgets(line, sizeof(line), input)) {
        ParsedLine parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_LABEL) {
            add_symbol(parsedLine.label, current_address);
        } else if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            current_address += 4;
        }
    }

    // Prepare for second pass
    fseek(input, 0, SEEK_SET); // Reset file pointer to beginning of input file

    // Second pass: Encode instructions
    while (fgets(line, sizeof(line), input)) {
        ParsedLine parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            encode_instruction(output, &parsedLine);
        }
    }
}

void write_magic_number(FILE *output) {
    int corewar_exec_magic = COREWAR_EXEC_MAGIC;
    unsigned char magic_number[4];

    // Hacky way to write the magic number to the output file in big-endian format

    magic_number[0] = (corewar_exec_magic >> 24) & 0xFF;
    magic_number[1] = (corewar_exec_magic >> 16) & 0xFF;
    magic_number[2] = (corewar_exec_magic >> 8) & 0xFF;
    magic_number[3] = corewar_exec_magic & 0xFF;

    fwrite(magic_number, sizeof(int), 1, output);
}

void write_program_size(FILE *input, FILE *output_file) {
    char line[MAX_LINE_LENGTH];
    int program_size = 0;

    while (fgets(line, sizeof(line), input)) {
        ParsedLine parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            program_size += 4;
        }
    }

    unsigned int big_endian_value = htonl(program_size);


    fwrite(&big_endian_value, sizeof(int), 1, output_file);

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

    write_magic_number(output_file);

    fwrite(header.name, sizeof(char), sizeof(char) * PROG_NAME_LENGTH, output_file);
    fwrite("\0\0\0\0", sizeof(char), 4, output_file); // Placeholder for program instructions size (4 bytes)

    write_program_size(input_file, output_file);

    fwrite(header.comment, sizeof(char), sizeof(char) * COMMENT_LENGTH, output_file);
    fwrite("\0\0\0\0", sizeof(char), 4, output_file); // Placeholder for program instructions size (4 bytes)

    assemble(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}