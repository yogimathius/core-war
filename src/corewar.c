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
    core_t vm;
    vm.instruction_pointer = 0;
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }
    while (ac > 1) {
        printf("Loading champion: %s\n", av[ac - 1]);
        champion_t *champ = init_champion();
        create_champion(champ, av[ac - 1]);
        add_champion(&vm, champ);
        ac--;
    }

    int i = 0;
    while (i < vm.champion_count) {
        printf("Champion P%d: %s\n", vm.champions[i].id, vm.champions[i].header.prog_name);
        run_champion(&vm, vm.champions[i]);
        i++;
    }

    return 0;
}
