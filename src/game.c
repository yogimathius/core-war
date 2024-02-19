#include <game.h>
#include <champion.h>

void run_game(core_t *core_vm) {
    int i = 0;
    printf("\n\n");
    printf("====================START GAME=====================\n");
    while (i < core_vm->champion_count) {
        printf("====================Champion P%d: %s=====================\n", core_vm->champions[i].id, core_vm->champions[i].header.prog_name);
        run_champion(core_vm, core_vm->champions[i]);
        i++;
    }
}