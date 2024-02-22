#include "../include/op.h"
#include "../include/instructions.h"

void print_operands(char **operands) {
    printf("Hexadecimal operands\n");
    int j = 0;
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
    champion->instruction_size = j;
    print_operands(operands);
    return operands;
}

instruction_t *check_next_instruction(const char *parsed_instruction, instruction_t *inst) {
    if (parsed_instruction != NULL) {
        instruction_t *next_inst = (instruction_t*)malloc(sizeof(instruction_t));            
        if (next_inst == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        inst->next = next_inst;
        inst = next_inst;
    } else {
        inst->next = NULL;
    }
    return inst;
}

int build_opcode(const char *parsed_instruction, instruction_t *inst) {
    enum op_types opcode = (enum op_types)strtol(parsed_instruction, NULL, 16);
    printf("opcode: %d\n", opcode);
    if ((int)opcode - 1 < 0 || (int)opcode - 1 > 16) {
        printf("Invalid opcode: %d\n", opcode);
        return 1;
    }
    inst->opcode = opcode - 1;
    return 0;
}

void add_operand(char **parsed_instructions, int *i, op_t operation, int *operands, int j) {
    if (parsed_instructions[*i] == NULL) {
        printf("Not enough operands for operation: %s\n", operation.mnemonique);
        operands[j] = 0;
        return;
    }
    while (strcmp(parsed_instructions[*i], "00") == 0) {
        (*i)++;
    }
    operands[j] = strtol(parsed_instructions[*i], NULL, 10);
    printf("added operand: %d\n", operands[j]);
    if (j < operation.nbr_args - 1) {
        (*i)++;
    }
}

int *parse_operands(char **parsed_instructions, int *i, int opcode) {
    op_t operation = op_tab[opcode];
    printf("checking operation %s for operands\n", operation.mnemonique);
    // skipping parameter description
    (*i)++;
    int *operands = (int*)malloc(operation.nbr_args * sizeof(int));
    if (operands == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    while(j < operation.nbr_args) {
        add_operand(parsed_instructions, i, operation, operands, j);
        j++;
    }
    return operands;
}

instruction_t *allocate_instruction(instruction_t **inst_ptr) {
    instruction_t *inst = *inst_ptr;
    if (inst == NULL) {
        inst = (instruction_t*)malloc(sizeof(instruction_t));
        if (inst == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        *inst_ptr = inst;
    }
    return inst;
}

void build_instructions(char **parsed_instructions, instruction_t **inst_ptr) {
    int is_opcode = 1;
    instruction_t *inst = allocate_instruction(inst_ptr);
    int i = 0;

    while (parsed_instructions[i] != NULL) {
        if (is_opcode == 1 && strcmp(parsed_instructions[i], "00") != 0) {
            if (build_opcode(parsed_instructions[i], inst) == 1) {
                is_opcode = 1;
                break;
            } else {
                is_opcode = 0;
            }
        } else {
            if (inst->opcode < 0 || inst->opcode > 16) {
                printf("Invalid opcode for operands: %d\n", inst->opcode);
                break;
            }

            inst->operands = parse_operands(parsed_instructions, &i, inst->opcode);
            is_opcode = 1;
            inst = check_next_instruction(parsed_instructions[i], inst);
        }
        i++;
    }
}

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *args) {
    int i = 0;
    const op_t *operation = &op_tab[opcode];
    printf("args in execute instructions: ");
    while (i < operation->nbr_args) {
        if (args[i] < 0 || args[i] > 100) {
            printf("Invalid instruction: %d\n", args[i]);
            return;
        }
        printf("%d ", args[i]);
        i++;
    }
    printf("\n");
    
    if (operation->inst != NULL) {
        printf("executing operation: %s\n", operation->mnemonique);
        operation->inst(champ, vm, opcode, args);
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}