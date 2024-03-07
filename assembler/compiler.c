#include "./op.h"

void assemble(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];

    rewind(input);
    printf("round two\n");
    while (fgets(line, sizeof(line), input)) {
        // printf("line: %s\n", line);s
        parsed_line_t parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            encode_instruction(output, &parsedLine);
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

    FileHeader header = parse_header(input_file);
    // Open output file and write header
    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    parse_contents(input_file, &header);

    write_magic_number(output_file);

    write_header(output_file, &header);

    assemble(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}