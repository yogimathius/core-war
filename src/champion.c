#include "stdio.h"
#include "../include/champion.h"

champion_t *champion_new() {
  champion_t *champ;

	if ((champ = (champion_t *)(malloc(sizeof(champion_t)))) == NULL)
		return (NULL);
	return (champ);
}

void champion_load(champion_t *champion, unsigned char *memory, int address) {

}

void champion_execute_instruction(champion_t *champion, unsigned char *memory) {

}
