#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "op.h"

champion_t *init_champion();

void add_champion(core_t *core_t, champion_t *champion);
// void champion_execute_instruction(champion_t *champion, unsigned char *memory);

#endif