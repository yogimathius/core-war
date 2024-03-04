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
        if (core_vm->champions[i].dead) {
            champs_left--;
        } else {
            champion_t *champ = &core_vm->champions[i];
            if (champ->lives == 0) {
                if (champs_left == 1) {
                    core_vm->winner = champ->id;
                    return champs_left;
                }
                champ->dead = 1;
                print_colored_text(33);
                printf("Champion P%d has no lives left. Champion is dead.\n", champ->id);
                printf("\033[0m");
                champs_left--;
            } else {
                champ->lives = 0;
            }
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

void print_results(core_t *core_vm, char *message) {
    printf("%s\n", message);

    champion_t *winner = find_winner(core_vm);

    print_colored_text(winner->color);
    printf("The player %d (%s) is done.\n", winner->id, winner->header.prog_name);
    printf("\033[0m");
}

int game_over(core_t *core_vm) {
    if (core_vm->lives >= NBR_LIVE || core_vm->nbr_cycles >= core_vm->cycle_to_die) {
        printf("Live count maxed out. Decreasing cycle to die by %d\n", CYCLE_DELTA);
        core_vm->cycle_to_die -= CYCLE_DELTA;
        core_vm->lives = 0;
        core_vm->winner = 0;
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

int get_label_address(champion_t champion, char *label) {
    for (int i = 0; i < champion.instruction_size; i++) {
        instruction_t inst = champion.inst[i];
        if (inst.opcode == atoi(label)) {
            return i;
        }
    }
    return -1;
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

    // check if operands contains a label

    int i = 0;
    printf("\nnum operands: %d\n", found_inst->num_operands);
    while (i < found_inst->num_operands) {
        printf("iterating through operands\n");
        operand_t operand = found_inst->operand_list[i];
        // printf("operand type: %d\n", operand->type);
        printf("operand value: %d\n", operand.value);
        if (operand.type == T_IND) {
            printf("operand is ind\n");
            if (operand.label != NULL) {
                int label_address = get_label_address(champion, operand.label);
                if (label_address == -1) {
                    printf("Label %s not found\n", operand.label);
                    return;
                } else {
                    printf("Label %s found at address %d\n", operand.label, label_address);
                }
                operand.value = label_address;
            }
        }
        i++;
    }

    execute_instruction(vm, &champion, found_inst->opcode, found_inst->operands);
}

void run_instruction(int i, core_t *core_vm, champion_t champion, int instruction_pointer) {
    int start_index = i * (MEM_SIZE / core_vm->champion_count);
    int instruction_size = champion.instruction_size;

    int instruction_index = instruction_pointer >= instruction_size ? (instruction_pointer % instruction_size) : instruction_pointer;
    int memory_index = start_index + instruction_index;

    int opcode = core_vm->memory[memory_index];
    if (opcode < 0 || opcode > 16) {
        printf("Invalid opcode for operands: %d\n", opcode);
        return;
    }
    execute_instruction(core_vm, &champion, opcode, champion.inst[instruction_index].operands);
}

void run_instructions(core_t *core_vm) {
    for (int i = 0; i < core_vm->champion_count; i++) {
        if (core_vm->champions[i].dead) {
            print_colored_text(37);
            printf("Champion P%d has no lives left. Cannot run.\n", core_vm->champions[i].id);
            printf("\033[0m");
        } else {
            run_instruction(i, core_vm, core_vm->champions[i], core_vm->instruction_pointer);
        }
    }
}

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    int game_loop_number = 0;


    while (1) {
        printf("===============GAME LOOP %d===============\n\n", game_loop_number);
        run_instructions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
        if (game_over(core_vm)) {
            break;
        }
    }
}