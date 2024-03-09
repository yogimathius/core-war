#include <game.h>
#include <champion.h>
#include <stdbool.h>
#include <op.h>

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
    if (core_vm->dump > 0 && core_vm->nbr_cycles >= core_vm->dump) {
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

void run_instruction(int start_index, core_t *core_vm, champion_t *champion) {
    int instruction_size = champion->instruction_size;
    int instruction_pointer = core_vm->instruction_pointer;

    int instruction_index = instruction_pointer >= instruction_size ? (instruction_pointer % instruction_size) : instruction_pointer;
    int memory_index = start_index + instruction_index;

    int opcode = core_vm->memory[memory_index];
    if (opcode < 0 || opcode > 16) {
        printf("Invalid opcode for operands: %d\n", opcode);
        return;
    }
    execute_instruction(core_vm, champion, opcode, champion->inst[instruction_index].operands);
}


void run_instructions(core_t *core_vm) {
    const process_t *head = core_vm->process;
    printf("Iterating through processes\n");
    const process_t *current = head;
    int i = 0;
    do {
        i++;
        run_instruction(current->index, core_vm, &core_vm->champions[current->player - 1]);
        current = current->next;
    } while (i < core_vm->champion_count);
}

int *hex_to_binary(int *bits, unsigned int hex) {
    for (int i=7;i>=0;i--) {
        bits[i]=hex&1;
        hex>>=1;
    }
    return bits;
}

char **split_binary(const int *bits, int size) {
    char **split = malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        split[i] = malloc(2 * sizeof(char));
        snprintf(split[i], 2, "%d", bits[i]);
    }
    return split;
}

enum op_types get_opcode(const int *temp_address, const core_t *core_vm) {
    enum op_types opcode = (enum op_types)strtol(core_vm->hex_memory[(*temp_address)], NULL, 16);

    return opcode;
}

int *build_arg_types(const op_t operation, char **split) {
    int *arg_types = malloc(operation.nbr_args * sizeof(int));
        for (int i = 0; i < operation.nbr_args; i++) {
            arg_types[i] = 0;
        }
        int j = 0;
        for (int i = 0; i < 8; i+=2) {
            char *binary_value = malloc(3 * sizeof(char)); // Allocate memory for 2 binary digits + '\0'
            snprintf(binary_value, 3, "%s%s", split[i], split[i+1]);
            if (operation.nbr_args == 0) {
                break;
            }
            if (j >= operation.nbr_args) {
                break;
            }
            switch (atoi(binary_value)) {
                case 01:
                    arg_types[j] = T_REG;
                    break;
                case 10:
                    arg_types[j] = T_DIR;
                    break;
                case 11:
                    arg_types[j] = T_IND;
                    break;  
                default:
                    break;
            }
            j++;
        }
    return arg_types;
}

int get_value(char **memory, int size, int start_index) {
    int value = 0;
    for (int i = 0; i < size; i++) {
        value += strtol(memory[start_index + i], NULL, 16);
    }
    return value;
}

int *build_operands(const op_t operation, char **hex_memory, int *temp_address) {
    int *operands = malloc(operation.nbr_args * sizeof(int));
    if (operation.nbr_args > 1) {
        int *bits = malloc(16 * sizeof(int));
        hex_to_binary(bits, strtol(hex_memory[(*temp_address)], NULL, 16));

        char **split = split_binary(bits, 8);
        const int *arg_types = build_arg_types(operation, split);
        (*temp_address)++;
        for (int i = 0; i < operation.nbr_args; i++) {
            if (arg_types[i] == T_REG) {
                operands[i] = strtol(hex_memory[(*temp_address)], NULL, 16);
                (*temp_address)++;
            } else if (arg_types[i] == T_DIR) {
                if (strcmp(hex_memory[((*temp_address))], "00") == 0 && strcmp(hex_memory[((*temp_address)) + 1], "00") == 0){
                    operands[i] = get_value(hex_memory, 4, (*temp_address));
                    (*temp_address)+=4;
                } else {
                    operands[i] = get_value(hex_memory, 2, (*temp_address));
                    (*temp_address)+=2;
                }
            } else if (arg_types[i] == T_IND) {
                operands[i] = get_value(hex_memory, 4, (*temp_address));
                (*temp_address)+=4;
            }
        }
        free(bits);
    } else {
        int value = get_value(hex_memory, 2, (*temp_address));

        // still have to determine why zjmp only has 2 bytes
        if ( hex_memory[((*temp_address)) + 2] != NULL) {
            value += strtol(hex_memory[((*temp_address)) + 2], NULL, 16);
            (*temp_address)++;
        }
        if ( hex_memory[((*temp_address)) + 3] != NULL) {
            value += strtol(hex_memory[((*temp_address)) + 3], NULL, 16);
            (*temp_address)++;
        }
        operands[0] = value;
        (*temp_address)+=2;
    }
    return operands;
}

void print_args_found(const op_t operation, const int *operands) {
    printf(" %d operand(s): ", operation.nbr_args);
    for (int i = 0; i < operation.nbr_args; i++) {
        printf("%d ", operands[i]);
    }
    printf("\n");
}

void run_hex_instruction(int *current_address, core_t *core_vm, process_t *process) {
    UNUSED(process);
    int temp_address = *current_address;
    enum op_types opcode = get_opcode(&temp_address, core_vm);

    if (opcode < 0 || opcode > 16) {
        printf("Invalid opcode for operands: %d\n", opcode);
        return;
    }
    temp_address++;
    printf("Opcode: %d\n", opcode);
    if (opcode == 0) {
        printf("Opcode is 0. Skipping instruction\n");
        core_vm->nbr_cycles+=10;
        return;
    }
    op_t operation = op_tab[opcode-1];
    int *operands = build_operands(operation, core_vm->hex_memory, &temp_address);
    execute_instruction(core_vm, find_champion(core_vm, process->player), opcode - 1, operands);
    *current_address = temp_address;
}

void run_hex_instructions(core_t *core_vm) {
    process_t *head = core_vm->process;
    process_t *current = head;
    int i = 0;
    do {
        i++;
        printf("\nP%d is currently executing ", current->player);
        champion_t *champion = find_champion(core_vm, current->player);
        int instruction_index = current->counter >= champion->parsed_instructions_size ? current->counter % champion->parsed_instructions_size : current->counter;


        int current_address = current->index + instruction_index;
        printf("at address: %d\n", current_address);
        run_hex_instruction(&current_address, core_vm, current);
        current->counter = current_address - current->index;
        current = current->next;
    } while (i < core_vm->champion_count);
}

void run_game(core_t *core_vm) {
    printf("\n\n====================START GAME=====================\n");
    int game_loop_number = 0;

    while (game_on(core_vm) == 0) {
        printf("===============GAME LOOP %d===============\n\n", game_loop_number);
        run_hex_instructions(core_vm);
        game_loop_number++;
        core_vm->instruction_pointer++;
    }
}