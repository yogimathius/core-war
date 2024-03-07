#include "./op.h"

FileHeader parse_header(FILE *input) {
    FileHeader header = {"", "", 0};
    char line[MAX_LINE_LENGTH];
    int name_found = 0;
    int comment_found = 0;
    while (fgets(line, sizeof(line), input) && (!name_found || !comment_found)) {
        if (strncmp(line, ".name", 5) == 0) {
            sscanf(line, ".name \"%[^\"]\"", header.name);
            name_found = 1;
        } else if (strncmp(line, ".comment", 8) == 0) {
            sscanf(line, ".comment \"%[^\"]\"", header.comment);
            comment_found = 1;
        }
    }
    rewind(input);

    return header;
}