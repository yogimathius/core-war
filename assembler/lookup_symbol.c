#include "./op.h"

Symbol *symbol_table = NULL;

// Function to add a symbol to the table
void add_symbol(const char *label, int address) {
    Symbol *symbol = malloc(sizeof(Symbol));
    strcpy(symbol->label, label);
    symbol->address = address;
    symbol->next = symbol_table;
    symbol_table = symbol;
}

// Function to look up a symbol in the table
int lookup_symbol(const char *label) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        printf("current->label: %s\n", current->label);
        printf("address: %d\n", current->address);
        if (strncmp(current->label, label, strlen(label)) == 0) {
            return current->address;
        }
        current = current->next;
    }
    return -1; // Not found
}