#include <op.h>
#include <vm.h>
#include <instructions.h>
#include <game.h>

int main(int ac, char **av) {
    printf("Starting corewar\n");
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }
    core_t *core_vm = init_vm();
    parse_args(core_vm, ac, av);

    run_game(core_vm);

    return 0;
}
