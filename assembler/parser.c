#include "./op.h"

FileHeader *init_header() {
    FileHeader *header = malloc(sizeof(FileHeader));
    header->size = 0;
    header->parsed_lines_size = 0;
    for (int i = 0; i < MAX_PROG_LENGTH; i++) {
        header->parsed_lines[i].lineType = TOKEN_UNKNOWN;
    }
    memset(header->name, 0, PROG_NAME_LENGTH + 2);
    memset(header->comment, 0, COMMENT_LENGTH + 1);
    return header;
}

void add_bytes_to_header(FileHeader *header, int arg_type) {
    switch (arg_type) {
        case TOKEN_REGISTER:
            header->size += REG_SIZE;
            break;
        case TOKEN_DIRECT:
            header->size += DIR_SIZE;
            break;
        case TOKEN_INDIRECT:
            header->size += IND_SIZE;
            break;
        default:
            break;
    }
}

parsed_line_t parse_line(const char *line, FileHeader *header, int current_address) {
    parsed_line_t parsedLine = {TOKEN_UNKNOWN, "", "", {""}, 0};
    const char *inputPtr = line;
    // printf("inputPtr: %s\n", inputPtr);
    Token token;

    // Check for empty line or comment
    token = lex_token(&inputPtr);
    if (token.type == TOKEN_BLANKLINE || token.type == TOKEN_COMMENT) {
        parsedLine.lineType = token.type;
        return parsedLine;
    }

    // Process the first token which could be a label or an instruction
    if (token.type == TOKEN_LABEL) {
        printf("found label: %s\n", token.string);
        parsedLine.lineType = TOKEN_LABEL;
        strcpy(parsedLine.label, token.string);
        add_symbol(parsedLine.label, current_address);
        token = lex_token(&inputPtr); // Get next token after label
    }

    if (token.type == TOKEN_INSTRUCTION) {
        parsedLine.lineType = TOKEN_INSTRUCTION;
        strcpy(parsedLine.opcode, token.string);
        header->size += 2; // Adding two to include the opcode and the parameter description byte
    } else if (token.type == TOKEN_DIRECT) {
        printf("found directive: %s\n", token.string);
        parsedLine.lineType = TOKEN_DIRECT;
        strcpy(parsedLine.opcode, token.string);
    }

    // Process arguments if any
    // printf("\n=======parsing arguments======\n");
    while ((token = lex_token(&inputPtr)).type != TOKEN_BLANKLINE && token.type != TOKEN_COMMENT) {
        if (parsedLine.argumentCount < MAX_ARGS_NUMBER && token.string[0] != '\0') {
            printf("arg token type: %d\n", token.type);
            // printf("found arg: %s\n", token.string);
            add_bytes_to_header(header, token.type);
            strcpy(parsedLine.arguments[parsedLine.argumentCount], token.string);
            parsedLine.argumentCount++;
        } else {
            // Handle error: too many arguments
        }
    }
    fflush(stdout);

    return parsedLine;
}

void parse_contents(FILE *input, FileHeader *header) {
    char line[MAX_LINE_LENGTH];
    int current_address = 0; 
    int instruction_count = 0;
    while (fgets(line, sizeof(line), input)) {
        if (strncmp(line, ".name", 5) == 0) {
            sscanf(line, ".name \"%[^\"]\"", header->name);
        } else if (strncmp(line, ".comment", 8) == 0) {
            sscanf(line, ".comment \"%[^\"]\"", header->comment);
        }
        parsed_line_t parsedLine = parse_line(line, header, current_address);

        if (parsedLine.lineType == TOKEN_LABEL) {
            add_symbol(parsedLine.label, current_address);
        } else if (parsedLine.lineType == TOKEN_INSTRUCTION) {  
            current_address = header->size;
            // header->size += 4;
            header->parsed_lines[instruction_count] = parsedLine;
            header->parsed_lines_size++;
            instruction_count++;
        }
    }
}
