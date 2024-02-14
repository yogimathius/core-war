#include "./op.h"

Symbol *symbol_table = NULL;

// Function to add a symbol to the table
void add_symbol(const char *label, int address) {
    Symbol *symbol = malloc(sizeof(Symbol));
    strcpy(symbol->label, label);
    symbol->address = address;
    symbol->next = symbol_table;
    symbol_table = symbol;
    printf("Added symbol: %s with address: %d\n", label, address); // Debug print
}

// Function to look up a symbol in the table
int lookup_symbol(const char *label) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        if (strcmp(current->label, label) == 0) {
            printf("Found symbol: %s at address: %d\n", label, current->address); // Debug print
            return current->address;
        }
        current = current->next;
    }
    return -1; // Not found
}