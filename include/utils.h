#ifndef UTILS_H_
#define UTILS_H_

#include <op.h>

void print_colored_text(int color);

void display_memory(const core_t *core);

void parse_args(core_t *core_vm, int ac, char **av);

#endif