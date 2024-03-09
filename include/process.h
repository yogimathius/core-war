#ifndef PROCESS_H_
#define PROCESS_H_

#include <champion.h>

process_t *init_process(const champion_t *champion, int index);

void build_processes(core_t *core);

#endif