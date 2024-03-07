#include "./op.h"

parsed_line_t parse_line(const char *line) {
    parsed_line_t parsedLine = {TOKEN_UNKNOWN, "", "", {""}, 0};
    const char *inputPtr = line;
    printf("inputPtr: %s\n", inputPtr);
    Token token;

    // Check for empty line or comment
    token = lex_token(&inputPtr);
    if (token.type == TOKEN_BLANKLINE || token.type == TOKEN_COMMENT) {
        parsedLine.lineType = token.type;
        return parsedLine;
    }

    // Process the first token which could be a label or an instruction
    if (token.type == TOKEN_LABEL) {
        parsedLine.lineType = TOKEN_LABEL;
        strcpy(parsedLine.label, token.string);
        token = lex_token(&inputPtr); // Get next token after label
    }

    if (token.type == TOKEN_INSTRUCTION) {
        parsedLine.lineType = TOKEN_INSTRUCTION;
        strcpy(parsedLine.opcode, token.string);
    } else if (token.type == TOKEN_DIRECTIVE) {
        parsedLine.lineType = TOKEN_DIRECTIVE;
        strcpy(parsedLine.opcode, token.string);
    }

    // Process arguments if any
    // printf("\n=======parsing arguments======\n");
    while ((token = lex_token(&inputPtr)).type != TOKEN_BLANKLINE && token.type != TOKEN_COMMENT) {
        if (parsedLine.argumentCount < MAX_ARGS_NUMBER && token.string[0] != '\0') {
            // printf("found arg: %s\n", token.string);
            strcpy(parsedLine.arguments[parsedLine.argumentCount], token.string);
            parsedLine.argumentCount++;
        } else {
            // Handle error: too many arguments
        }
    }
    fflush(stdout);

    return parsedLine;
}
