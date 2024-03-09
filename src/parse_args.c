#include <op.h>
#include <instructions.h>
#include <champion.h>
#include <vm.h>

void parse_args(core_t *core_vm, int ac, char **av) {
    if (ac < 2) {
        printf("Usage: ./corewar [champion1.cor] [champion2.cor] [champion3.cor] [champion4.cor]\n");
        exit(1);
    }

    int set_id;
    printf("ac: %d\n", ac);
    for (int i = 1; i < ac; i++) {
      printf("av[i]: %s\n", av[i]);
        if (strstr(av[i], "-") != NULL){
            if (strcmp(av[i], "-dump") == 0) {
                if (i + 1 < ac) {
                    core_vm->dump = atoi(av[i + 1]);
                    i++;
                } else {
                    printf("Invalid dump value\n");
                    exit(1);
                }
            } else if (strcmp(av[i], "-n") == 0) {
                if (i + 1 < ac) {
                    set_id = atoi(av[i + 1]);
                    i++;
                } else {
                    printf("Invalid champion id\n");
                    exit(1);
                }
            } else {
                printf("Invalid argument: %s\n", av[i]);
                exit(1);
            }
        } else if (strstr(av[i], ".cor") != NULL) {
            printf("Loading champion: %s\n", av[i]);
            champion_t *champ = init_champion();
            if (set_id) {
                champ->id = set_id;
                set_id = 0;
            }
            create_champion(champ, av[i ]);
            add_champion(core_vm, champ);
        }
    }
    load_instructions(core_vm);
    build_processes(core_vm);
}