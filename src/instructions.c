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

void build_instructions(char **parsed_instructions, instruction_t **inst_ptr) {
    int is_opcode = 1;
    instruction_t *inst = *inst_ptr;
    if (inst == NULL) {
        inst = (instruction_t*)malloc(sizeof(instruction_t));
        if (inst == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        *inst_ptr = inst;
    }
    int i = 0;

    while (parsed_instructions[i] != NULL) {
        if (is_opcode == 1) {
            is_opcode = 0;
            enum op_types opcode = (enum op_types)strtol(parsed_instructions[i], NULL, 16);
            inst->opcode = opcode - 1;
        } else {
            if (inst->opcode < 0 || inst->opcode > 16) {
                printf("Invalid opcode: %d\n", inst->opcode);
                break;
            }
            op_t operation = op_tab[inst->opcode];
            
            printf("checking operation for operands: %s\n", operation.mnemonique);
            // skipping parameter description
            i++;
            inst->operands = (int*)malloc(operation.nbr_args * sizeof(int));
            if (inst->operands == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            int j = 0;
            int added_operand = 0;
            while(j < operation.nbr_args) {
                // chceking if the operand is a register
                if (strcmp(parsed_instructions[i], "00") == 0 && added_operand == 1){
                    i++;
                    if (strcmp(parsed_instructions[i+1], "00") == 0) {
                        i++;
                    } else {
                        added_operand = 0;
                    }
                }
                inst->operands[j] = strtol(parsed_instructions[i], NULL, 10);
                added_operand = 1;
                printf("added operand: %d\n", inst->operands[j]);
                if (j < operation.nbr_args - 1) {
                    i++;
                }
                j++;
            }
            is_opcode = 1;
            if (parsed_instructions[i] != NULL) {
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