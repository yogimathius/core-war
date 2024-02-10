#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/op.h"
#include "../include/vm.h"
#include "../include/instructions.h"

int main(int ac, char **av) {
    printf("Starting corewar\n");
    core_t *core_vm = init_vm();
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }
    printf("====================LOADING CHAMPIONS=====================\n");

    while (ac > 1) {
        printf("Loading champion: %s\n", av[ac - 1]);
        champion_t *champ = init_champion();
        create_champion(champ, av[ac - 1]);
        add_champion(core_vm, champ);
        ac--;
    }

    int i = 0;
    printf("\n\n");
    printf("====================START GAME=====================\n");
    while (i < core_vm->champion_count) {
        printf("Champion P%d: %s\n", core_vm->champions[i].id, core_vm->champions[i].header.prog_name);
        run_champion(core_vm, core_vm->champions[i]);
        i++;
    }

    return 0;
}
