#include <op.h>
#include <vm.h>
#include <instructions.h>

int main(int ac, char **av) {
    printf("Starting corewar\n");
    core_t *core_vm = init_vm();
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }
    
    load_champions(core_vm, ac, av);

    int i = 0;
    printf("\n\n");
    printf("====================START GAME=====================\n");
    while (i < core_vm->champion_count) {
        printf("====================Champion P%d: %s=====================\n", core_vm->champions[i].id, core_vm->champions[i].header.prog_name);
        run_champion(core_vm, core_vm->champions[i]);
        i++;
    }

    return 0;
}
