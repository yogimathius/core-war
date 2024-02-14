#include "./op.h"

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