#include "stdio.h"
#include "stdlib.h"
#include "../include/champion.h"

champion_t *init_champion(int id) {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);

  champ->id = id;

	return champ;
}

// int add_champion(core_t *core_t, champion_t *champion) {
//   unused(champion);
//   unused(core_t);
//   return 0;
// }

// void champion_execute_instruction(champion_t *champion, unsigned char *memory) {

// }
