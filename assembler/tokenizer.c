#include "./op.h"

Token lex_token(const char **input) {
    Token token = {TOKEN_UNKNOWN, ""};
    const char *start = *input;

    // Skip whitespace
    while (isspace((unsigned char)**input)) {
        (*input)++;
    }

    if (**input == '\0') {
        token.type = TOKEN_BLANKLINE;
    } else if (**input == ';') { // Assuming ';' starts a comment
        token.type = TOKEN_COMMENT;
        while (**input && **input != '\n') {
            (*input)++;
        }
    } else if (**input == '.') { // Directive
        token.type = TOKEN_DIRECTIVE;
        (*input)++;
        start = *input; // Start after the '.'
        while (!isspace((unsigned char)**input) && **input != '\0') {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';

        // Skip whitespace after directive keyword
        while (isspace((unsigned char)**input)) {
            (*input)++;
        }

        // Store the rest of the line as an argument (if present)
        start = *input;
        while (**input != '\n' && **input != '\0') {
            (*input)++;
        }
        if (start != *input) { // There's additional text after the directive keyword
            length = *input - start;
            if (length > MAX_ARGUMENT_LENGTH - 1) {
                length = MAX_ARGUMENT_LENGTH - 1; // Limit the length to avoid buffer overflow
            }
            strncpy(token.string, start, length);
            token.string[length] = '\0';
        }
    } else if (isalpha((unsigned char)**input) && *input[0] != 'r') { // Label or Instruction
        // printf("input in aslpha: %s\n", *input);

        start = *input;
        while (isalnum((unsigned char)**input) || **input == '_') {
            (*input)++;
        }
        if (**input == LABEL_CHAR) { // It's a label
        // printf("input in label: %s\n", *input);
            token.type = TOKEN_LABEL;
            (*input)++;
        } else { // It's an instruction
            token.type = TOKEN_INSTRUCTION;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (isdigit((unsigned char)**input) || **input == '-') { // Number
        printf("input from digit: %s\n", *input);
        token.type = TOKEN_NUMBER;
        if (**input == '-') {
            (*input)++;
        }
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (*input[0] == 'r' && isdigit((unsigned char)*(*input + 1))) { // Register
        token.type = TOKEN_REGISTER;
        // printf("input from r: %s\n", *input);
        start = *input; // Start at the digit
        (*input)++; // Skip 'r'
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    } else if (**input == DIRECT_CHAR && *(*input + 1) == LABEL_CHAR) { // Direct label
        // printf("input from direct label: %s\n", *input);
        token.type = TOKEN_DIRECTIVE;
        start = *input; // Start after '%:'
        (*input) += 2; // Skip '%:'
        while (isalnum((unsigned char)**input) || **input == '_') {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }
    else if (**input == '%' && isdigit((unsigned char)*(*input + 1))) { // Direct value
        token.type = TOKEN_NUMBER; // or a new type, e.g., TOKEN_DIRECT_VALUE
        start = *input; // Start at the digit
        (*input)++; // Skip '%'
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }   
    else if (**input == ',') {  // Comma separator
        token.type = TOKEN_SEPARATOR;
        (*input)++;  // Skip the comma
    }
    else if (*input[0] == 'r' && isdigit((unsigned char)*(*input + 1))) { // Register
        printf("input: %s\n", *input);
        token.type = TOKEN_REGISTER;
        (*input)++; // Skip 'r'
        start = *input; // Start at the digit
        while (isdigit((unsigned char)**input)) {
            (*input)++;
        }
        size_t length = *input - start;
        strncpy(token.string, start, length);
        token.string[length] = '\0';
    }

    return token;
}
