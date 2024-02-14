#include "./op.h"

ParsedLine parse_line(const char *line) {
    ParsedLine parsedLine = {TOKEN_UNKNOWN, "", "", {""}, 0};
    const char *inputPtr = line;

    Token token;

    // Check for empty line or comment
    token = lex_token(&inputPtr);
    printf("token type: %i\n", token.type);
    printf("token string: %s\n", token.string);
    if (token.type == TOKEN_ENDLINE || token.type == TOKEN_COMMENT) {
        parsedLine.lineType = token.type;
        return parsedLine;
    }

    // Process the first token which could be a label or an instruction
    if (token.type == TOKEN_LABEL) {
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
    printf("parsedLine.opcode: %s\n", parsedLine.opcode);

    // Process arguments if any
    while ((token = lex_token(&inputPtr)).type != TOKEN_ENDLINE && token.type != TOKEN_COMMENT) {
        printf("token type: %i\n", token.type);
        if (parsedLine.argumentCount < MAX_ARGS_NUMBER) {
            strcpy(parsedLine.arguments[parsedLine.argumentCount++], token.string);
        } else {
            // Handle error: too many arguments
        }
    }
    printf("parsedLine.argumentCount: %i\n", parsedLine.argumentCount);
    fflush(stdout);

    return parsedLine;
}
