#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "op.h"
#include "instructions.h"

champion_t *init_champion();

champion_t *create_champion(champion_t *champion, char *filename);

void add_champion(core_t *core_t, champion_t *champion);

void run_champion(core_t *vm, champion_t champion);

void load_champions(core_t *core_vm, int ac, char **av);

void run_champions(core_t *vm);

instruction_t* deep_copy_instructions(const instruction_t* original);

champion_t* clone_champion(const champion_t* original);

int add_champion_to_core(core_t* core, champion_t* champion);

#endif