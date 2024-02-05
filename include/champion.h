#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "op.h"

champion_t *init_champion();

champion_t *create_champion(champion_t *champion, char *filename);

void add_champion(core_t *core_t, champion_t *champion);

#endif