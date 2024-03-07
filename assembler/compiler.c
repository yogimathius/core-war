#include "./op.h"

void parse_contents(FILE *input, FileHeader *header) {
    char line[MAX_LINE_LENGTH];
    int current_address = 0; 

    while (fgets(line, sizeof(line), input)) {
        parsed_line_t parsedLine = parse_line(line);
        if (parsedLine.lineType == TOKEN_LABEL) {
            add_symbol(parsedLine.label, current_address);
        } else if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            current_address += 1 + parsedLine.argumentCount;
            header->size += 4;
        }
    }

    // Prepare for second pass
    rewind(input);
}

void assemble(FILE *input, FILE *output) {
    char line[MAX_LINE_LENGTH];
    int current_address = 0; 
    printf(" %d\n", current_address);
    fseek(input, 0, SEEK_SET); // Reset file pointer to beginning of input file

    printf("round one\n");
    while (fgets(line, sizeof(line), input)) {
        parsed_line_t parsedLine = parse_line(line);
        printf("parsedLine.lineType: %d\n", parsedLine.lineType);

        if (parsedLine.lineType == TOKEN_LABEL) {
            printf("adding symbol: %s, %d\n", parsedLine.label, current_address);
            add_symbol(parsedLine.label, current_address);
        } else if (parsedLine.lineType == TOKEN_INSTRUCTION) {
            current_address += 1 + parsedLine.argumentCount;
        }
    }

    // Prepare for second pass
    rewind(input);
    printf("round two\n");
    // Second pass: Encode instructions
    while (fgets(line, sizeof(line), input)) {
        // printf("line: %s\n", line);s
        parsed_line_t parsedLine = parse_line(line);
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

void write_header(FILE *output, FileHeader *header) {
    fwrite(header->name, sizeof(char), sizeof(char) * PROG_NAME_LENGTH, output);
    fwrite("\0\0\0\0", sizeof(char), 4, output); // Placeholder for program instructions size (4 bytes)

    unsigned int size = htonl(header->size);
    fwrite(&size, sizeof(int), 1, output);

    fwrite(header->comment, sizeof(char), sizeof(char) * COMMENT_LENGTH, output);
    fwrite("\0\0\0\0", sizeof(char), 4, output); // Placeholder for program instructions size (4 bytes)
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