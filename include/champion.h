#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "op.h"
#include "instructions.h"
#include "vm.h"
#include "utils.h"

champion_t *init_champion();

champion_t *create_champion(champion_t *champion, char *filename);

void add_champion(core_t *core_t, champion_t *champion);

champion_t* clone_champion(const champion_t* original);

int add_champion_to_core(core_t* core, champion_t* champion);

champion_t *find_champion(core_t *core_vm, int id);

int check_champion_lives(core_t *core_vm);

#endif