#include <game.h>
#include <champion.h>

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    // Phase 5: we want each champion to call ONLY ONE instruction per turn.
    int game_loop_number = 0;
    int max_game_loops = 10;

    while (game_loop_number < max_game_loops) {
        printf("===============GAME LOOP %d===============\n\n", game_loop_number);
        run_champions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
    }
}