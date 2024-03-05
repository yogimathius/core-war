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

char **parse_instructions(const char *hex_buffer, int bytes_read, champion_t *champion) {
    char **operands = (char**)malloc((MEM_SIZE + 1) * sizeof(char*));
    allocate_operands(operands);

    int j = 0;
    for (int i = 140 + COMMENT_LENGTH + 4; i < bytes_read && j < MEM_SIZE; i++) {
      snprintf(operands[j], 3, "%.2x", (unsigned int)hex_buffer[i]);
      j++;
    }
    operands[j] = NULL;
    champion->parsed_instructions_size = j;
    print_operands(operands);
    return operands;
}

int build_opcode(const char *parsed_instruction, instruction_t *inst) {
    enum op_types opcode = (enum op_types)strtol(parsed_instruction, NULL, 16);
    if ((int)opcode - 1 < 0 || (int)opcode - 1 > 16) {
        printf("Invalid opcode: %d\n", opcode);
        return 1;
    }
    inst->opcode = opcode - 1;
    return 0;
}

void skip_zeroes(char **parsed_instructions, int *i) {
    if (strcmp(parsed_instructions[*i], "00") == 0) {
        // for 2 byte operands
        (*i)++;
        if (strcmp(parsed_instructions[*i], "00") == 0) {
            (*i)++;
            (*i)++;
            printf("bumping to next operand: %s\n", parsed_instructions[*i]);
        }
    }
}

void create_operand(const int *i, char **parsed_instructions, int *found_label_address, int j, instruction_t *inst) {
    unsigned int hex_value = strtol(parsed_instructions[*i], NULL, 16);
    printf("Hex value: 0x%X\n", hex_value);
    operand_t *operand = &inst->operand_list[j];
    operand->value = hex_value;

    if (hex_value & T_REG) {
        printf("T_REG is set: %u\n", hex_value);
        operand->type = T_REG;
        operand->label = NULL;
    } else if (hex_value & T_IND) {
        printf("T_IND is set, address of label: %s\n", 
        parsed_instructions[hex_value]);
        printf("int hex_value: %u\n", hex_value);

        operand->type = T_IND;
        operand->label = NULL;
    } else if(hex_value & T_DIR) {
        printf("T_DIR is set: %u\n", hex_value);
        operand->type = T_DIR;
        printf("int hex_value: %u\n", hex_value);
        if (hex_value > 0 && hex_value < 16) {
            *found_label_address = (int)hex_value;
        }
        operand->label = parsed_instructions[hex_value];
        // int k = hex_value;
        // while (parsed_instructions[k] != NULL) {
        //     operand->label = parsed_instructions[k];
        //     operand->index = j;
        //     k++;
        //     operand->label++;
        // }
    }

    inst->operand_list[j] = *operand;
}

void add_operand(char **parsed_instructions, int *i, op_t operation, int *operands, int j, instruction_t *inst, int *found_label_address) {
    printf("checking operand: %s\n", parsed_instructions[*i]);
    if (parsed_instructions[*i] == NULL) {
        printf("Not enough operands for operation: %s\n", operation.mnemonique);
        operands[j] = 0;
        return;
    }

    skip_zeroes(parsed_instructions, i);
    create_operand(i, parsed_instructions, found_label_address, j, inst);

    operands[j] = strtol(parsed_instructions[*i], NULL, 10);

    if (j < operation.nbr_args - 1) {
        (*i)++;
    }
}

int *build_operands(char **parsed_instructions, int *i, instruction_t *inst, int *found_label_address) {
    op_t operation = op_tab[inst->opcode];
    (*i)++;
    printf("\nChecking Operation: %s\n", operation.mnemonique);
    inst->num_operands = 0;
    int *operands = (int*)malloc(operation.nbr_args * sizeof(int));
    inst->operand_list = (operand_t*)malloc(operation.nbr_args * sizeof(operand_t));
    if (operands == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    while(j < operation.nbr_args) {
        add_operand(parsed_instructions, i, operation, operands, j, inst, found_label_address);
        j++;
        inst->num_operands++;
    }
    return operands;
}

instruction_t *allocate_instruction() {
    instruction_t *inst = (instruction_t*)malloc(sizeof(instruction_t));
    if (inst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return inst;
}

int build_instructions(char **parsed_instructions, champion_t *champion) {
    int is_opcode = 1;
    instruction_t *inst = allocate_instruction();
    int i = 0;
    int instruction_size = 0;
    int found_label_address = 0;

    while (parsed_instructions[i] != NULL) {
        if (found_label_address > 0 && found_label_address <= i) {
            printf("breaking, found_label_address: %d\n", found_label_address);
            break;
        }
        if (is_opcode == 1 && strcmp(parsed_instructions[i], "00") != 0) {
            if (build_opcode(parsed_instructions[i], inst) == 1) {
                is_opcode = 1;
                break;
            } else {
                is_opcode = 0;
                instruction_size++;
            }
        } else {
            if (inst->opcode < 0 || inst->opcode > 16) {
                printf("Invalid opcode for operands: %d\n", inst->opcode);
                break;
            }

            inst->operands = build_operands(parsed_instructions, &i, inst, &found_label_address);
            is_opcode = 1;
            champion->inst[instruction_size - 1] = *inst;
        }
        i++;
    }
    return instruction_size;
}

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *args) {
    const op_t *operation = &op_tab[opcode];
    vm->nbr_cycles += operation->nbr_cycles;
    // delay(operation->nbr_cycles);
    if (operation->inst != NULL) {
        operation->inst(champ, vm, opcode, args);
        champ->counter++;
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}