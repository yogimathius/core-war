#include "./op.h"

void assemble(FILE *output, FileHeader *header) {
    int current_address = 0;
    for (int i = 0; i < header->parsed_lines_size; i++) {
        parsed_line_t parsedLine = header->parsed_lines[i];
        if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            encode_instruction(output, &parsedLine, &current_address);
        }
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

    FileHeader *header = init_header();

    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    parse_contents(input_file, header);

    encode_header(output_file, header);

    assemble(output_file, header);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}