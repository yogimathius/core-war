#include <game.h>
#include <champion.h>
#include <stdbool.h>

champion_t *find_champion(core_t *core_vm, int id) {
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        if (core_vm->champions[i].id == id) {
            return &core_vm->champions[i];
        }
    }
    return &core_vm->champions[0];
}

int check_champion_lives(core_t *core_vm) {
    int champs_left = core_vm->champion_count;
     for (int i = 0; i < core_vm->champion_count; i++) {
        champion_t *champ = &core_vm->champions[i];
        if (champ->lives == 0) {
            if (core_vm->champion_count == 1) {
                core_vm->winner = champ->id;
                return champs_left;
            }
            champ->dead = 1;
            print_colored_text(33);
            printf("Champion P%d has no lives left. Champion is dead.\n", champ->id);
            printf("\033[0m");
            core_vm->champion_count--;
        } else {
            champ->lives = 0;
        }
    }
    
    return champs_left;
}

champion_t *find_winner(core_t *core) {
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        if (core->champions[i].dead == 0) {
            return &core->champions[i];
        }
    }
    return &core->champions[0];
}

int game_over(core_t *core_vm) {
    if (core_vm->lives >= NBR_LIVE || core_vm->nbr_cycles >= core_vm->cycle_to_die) {
        printf("Live count maxed out. Decreasing cycle to die by %d\n", CYCLE_DELTA);
        core_vm->cycle_to_die -= CYCLE_DELTA;
        core_vm->lives = 0;
        core_vm->winner = 0;
        int champs_left = check_champion_lives(core_vm);
        if (champs_left == 1) {
            printf("All other champions have been defeated!!\n");

            champion_t *winner = find_winner(core_vm);

            print_colored_text(winner->color);
            printf("The player %d (%s) is done.\n", winner->id, winner->header.prog_name);
            printf("\033[0m");
            return 1;
        }
        if (core_vm->cycle_to_die <= 0) {
            printf("Cycle to die is 0. Game over.\n");
            champion_t *winner = find_champion(core_vm, core_vm->winner);

            print_colored_text(winner->color);
            printf("The player %d (%s) is done.\n", winner->id, winner->header.prog_name);
            printf("\033[0m");
            return 1;
        }
    }
    return 0;
}

void run_champion(core_t *vm, champion_t champion) {
    int instruction_size = champion.instruction_size;
    int instruction_pointer = vm->instruction_pointer;

    int instruction_index = instruction_pointer >= instruction_size ? (instruction_pointer % instruction_size) : instruction_pointer;

    instruction_t *found_inst = &champion.inst[instruction_index];

    if (found_inst->opcode < 0 || found_inst->opcode > 16) {
        printf("cannot run champion. Invalid opcode for operands: %d\n", found_inst->opcode);
        return;
    }

    execute_instruction(vm, &champion, found_inst->opcode, found_inst->operands);
}

void run_champions(core_t *vm) {
    for (int i = 0; i < vm->champion_count; i++) {
      if (vm->champions[i].dead) {
            print_colored_text(37);
            printf("Champion P%d has no lives left. Cannot run.\n", vm->champions[i].id);
            printf("\033[0m");
      } else {
        run_champion(vm, vm->champions[i]);
      }
    }
}

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    int game_loop_number = 0;

    while (1) {
        printf("===============GAME LOOP %d===============\n\n", game_loop_number);
        run_champions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
        if (game_over(core_vm)) {
            break;
        }
    }
}