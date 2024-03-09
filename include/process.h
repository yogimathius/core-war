#ifndef PROCESS_H_
#define PROCESS_H_

#include <champion.h>

process_t *init_process(const champion_t *champion, int index);

void build_processes(core_t *core);

process_t *find_process(core_t *core, int player);

process_t *clone_process(const process_t *original, int new_address);

void fork_process(process_t *original, process_t *new_process);

#endif