#include <game.h>
#include <champion.h>
#include <stdbool.h>
#include <op.h>

champion_t *find_winner(core_t *core) {
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        if (core->champions[i].dead == 0) {
            return &core->champions[i];
        }
    }
    return &core->champions[0];
}

void print_results(core_t *core_vm, const char *message) {
    printf("%s\n", message);

    champion_t *winner = find_winner(core_vm);

    print_colored_text(winner->color);
    printf("The player %d (%s) is done.\n", winner->id, winner->header.prog_name);
    printf("\033[0m");
}

void reset_champ_counters(core_t *core_vm) {
    for (int i = 0; i < core_vm->champion_count; i++) {
        core_vm->champions[i].counter = 0;
    }
}

int game_on(core_t *core_vm) {
    if (core_vm->dump > 0 && core_vm->nbr_cycles == core_vm->dump) {
        printf("Dumping memory at cycle %d\n", core_vm->nbr_cycles);
        display_memory(core_vm);
    }
    if (core_vm->lives >= NBR_LIVE || core_vm->nbr_cycles >= core_vm->cycle_to_die) {
        printf("Live count maxed out. Decreasing cycle to die by %d\n", CYCLE_DELTA);
        // display_memory(core_vm);
        core_vm->cycle_to_die -= CYCLE_DELTA;
        core_vm->nbr_cycles = 0;
        core_vm->lives = 0;
        core_vm->winner = 0;
        reset_champ_counters(core_vm);
        int champs_left = check_champion_lives(core_vm);
        if (champs_left == 1) {
            print_results(core_vm, "All other champions have been defeated!!\n");
            return 1;
        }
        if (core_vm->cycle_to_die <= 0) {
            print_results(core_vm, "Cycle to die is 0. Game over.\n");
            return 1;
        }
    }
    return 0;
}

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    int game_loop_number = 0;

    while (game_on(core_vm) == 0) {
        printf("\n===============GAME LOOP %d===============\n\n", game_loop_number);
        run_instructions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
    }
}