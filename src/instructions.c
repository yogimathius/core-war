#include "../include/op.h"
#include "../include/instructions.h"
#include <time.h>
 
void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;
 
    clock_t start_time = clock();
 
    while (clock() < start_time + milli_seconds)
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

char **parse_instructions(const char *hex_buffer, int bytes_read) {
    char **operands = (char**)malloc((MEM_SIZE + 1) * sizeof(char*));
    allocate_operands(operands);

    int j = 0;
    for (int i = 140 + COMMENT_LENGTH + 4; i < bytes_read && j < MEM_SIZE; i++) {
      snprintf(operands[j], 3, "%.2x", (unsigned int)hex_buffer[i]);
      j++;
    }
    operands[j] = NULL;

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

    if (j < operation.nbr_args - 1) {
        (*i)++;
    }
}

int *parse_operands(char **parsed_instructions, int *i, int opcode) {
    op_t operation = op_tab[opcode];
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

int build_instructions(char **parsed_instructions, instruction_t **inst_ptr, champion_t *champion) {
    int is_opcode = 1;
    instruction_t *inst = allocate_instruction(inst_ptr);
    int i = 0;
    int instruction_size = 0;
    while (parsed_instructions[i] != NULL) {
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

            inst->operands = parse_operands(parsed_instructions, &i, inst->opcode);
            is_opcode = 1;
            champion->inst[instruction_size - 1] = *inst;
            inst = check_next_instruction(parsed_instructions[i], inst);
        }
        i++;
    }
    return instruction_size;
}

void execute_instruction(core_t *vm, champion_t *champ, enum op_types opcode, int *args) {
    const op_t *operation = &op_tab[opcode];
    vm->nbr_cycles += operation->nbr_cycles;
    delay(operation->nbr_cycles);
    if (operation->inst != NULL) {
        operation->inst(champ, vm, opcode, args);
    } else {
        printf("Unknown or unimplemented operation\n");
    }
}