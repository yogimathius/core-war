#include <game.h>
#include <champion.h>
#include <ncurses.h>
#include <stdbool.h>

champion_t *find_champion(core_t *core_vm, int id) {
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        if (core_vm->champions[i].id == id) {
            return &core_vm->champions[i];
        }
    }
    return &core_vm->champions[0];
}

int game_over(core_t *core_vm) {
    if (core_vm->lives >= NBR_LIVE || core_vm->nbr_cycles >= core_vm->cycle_to_die) {
        printf("Live count maxed out. Decreasing cycle to die by %d\n", CYCLE_DELTA);
        core_vm->cycle_to_die -= CYCLE_DELTA;
        core_vm->lives = 0;
        core_vm->winner = 0;
        if (core_vm->cycle_to_die <= 0) {
            return 1;
        }
    }
    return 0;
}

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    // Phase 5: we want each champion to call ONLY ONE instruction per turn.
    // If, after NBR_LIVE executions of the instruction live, several processes are still alive, CYCLE_TO_DIE is decreased by CYCLE_DELTA units. This starts over until there are no live processes left.
    int game_loop_number = 0;

    while (1) {
        printf("===============GAME LOOP %d===============\n\n", game_loop_number);
        run_champions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
        if (game_over(core_vm)) {
            printf("Game over\n");
            initscr();
            start_color();
            init_pair(1, COLOR_RED, COLOR_BLACK);
            printw("Cycle to die is 0. Game over.\n");
            champion_t *winner = find_champion(core_vm, core_vm->winner);
            attron(COLOR_PAIR(1));
            printw("The player %d (%s) is done.\n", winner->id, winner->header.prog_name);
            attroff(COLOR_PAIR(1));
            refresh();
            getch();

            endwin();
            break;
        }
    }
}