#include "stdio.h"
#include "stdlib.h"
#include "../include/champion.h"

champion_t *init_champion() {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);

	return champ;
}

void add_champion(core_t *core_t, champion_t *champion) {
  core_t->champion_count++;
  champion->id = core_t->champion_count;
  core_t->champions[core_t->champion_count - 1] = *champion;
  champion->counter = 0;
  champion->carry_flag = 0;
}
