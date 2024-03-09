#include <instructions.h>
#include <time.h>
 
void delay(int number_of_seconds) {
    int milli_seconds = 10 * number_of_seconds;
 
    clock_t start_time = clock();
    clock_t end_time = start_time + (milli_seconds * CLOCKS_PER_SEC / 1000);
 
    while (clock() < end_time)
        ;
}

void print_operands(char **operands) {
    int j = 0;
    printf("Hexadecimal operands\n");

    while (operands[j] != NULL) {
      printf("%s ", operands[j]);
      j++;
    }
    printf("\n");
}

void allocate_operands(char **operands) {
    if (operands == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MEM_SIZE + 1; i++) {
      operands[i] = malloc(3 * sizeof(char));

      if (operands[i] == NULL) {
          printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
      }
    }
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

void parse_instructions(const char *hex_buffer, int bytes_read, champion_t *champion) {
    char **operands = (char**)malloc((MEM_SIZE + 1) * sizeof(char*));
    allocate_operands(operands);

    int j = 0;
    for (int i = 140 + COMMENT_LENGTH + 4; i < bytes_read && j < MEM_SIZE; i++) {
        operands[j] = (char*)malloc(3 * sizeof(char));
        if (operands[j] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        snprintf(operands[j], 3, "%.2hhx", hex_buffer[i]);
      j++;
    }
    operands[j] = NULL;
    champion->parsed_instructions_size = j;
    print_operands(operands);
    champion->parsed_instructions = operands;
}

void run_instruction(int *current_address, core_t *core_vm, process_t *process) {
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
    log_instruction_args(find_champion(core_vm, process->player), core_vm, opcode - 1, operands);

    *current_address = temp_address;
}

void run_instructions(core_t *core_vm) {
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
        run_instruction(&current_address, core_vm, current);
        current->counter = current_address - current->index;
        current = current->next;
    } while (i < core_vm->champion_count);
}

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *args) {
    const op_t *operation = &op_tab[opcode];
    vm->nbr_cycles += operation->nbr_cycles;
    // delay(operation->nbr_cycles);
    if (operation->inst != NULL) {
        operation->inst(champ, vm, opcode, args);
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}