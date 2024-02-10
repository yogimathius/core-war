#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/op.h"
#include "../include/instructions.h"

char **parse_instructions(char *instruction) {
    char **operands = malloc((MEM_SIZE + 1) * sizeof(char *)); 
    if (operands == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(instruction, " ");
    int i = 0;

    while (token != NULL && i < MEM_SIZE) {
        operands[i] = strdup(token);
        if (operands[i] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
        i++;
    }

    operands[i] = NULL;

    return operands;
}

void build_instructions(champion_t *champ, char **parsed_instructions, instruction_t **inst_ptr) {
    instruction_t *inst = *inst_ptr;
    if (inst == NULL) {
        inst = (instruction_t*)malloc(sizeof(instruction_t));
        if (inst == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        *inst_ptr = inst;
    }
    printf("====================Building instruction: %s=====================\n", *parsed_instructions);
    enum op_types opcode = (enum op_types)strtoul(*parsed_instructions, NULL, 16);
    if (opcode < 1 || opcode > 16) {
        printf("No more valid instructions to parse\n");
        inst->next = NULL;
    } else {
        op_t operation = op_tab[opcode - 1];
        int operation_args = operation.nbr_args;
        inst->operands = (int*)malloc(operation_args * sizeof(int));
        if (inst->operands == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        // printf("opcode: %d\n", opcode);
        inst->opcode = opcode - 1;

        int i = 0;
        printf("opcode: %d, mnemonic: %s, args: %d\n", opcode, operation.mnemonique, operation_args);

        printf("operands: ");
        while (i < operation_args + 2) {
            if (i == 0 || i == 1) {
                parsed_instructions++;
                i++;
                continue;
            }
            long int operandValue = strtol(*parsed_instructions, NULL, 10);

            if (operandValue == 0 && **parsed_instructions != '0') {
                printf("Invalid operand: %s\n", *parsed_instructions);
                break;
            } else if (operandValue < 0 || operandValue > 100) {
                printf("Invalid register: %ld\n", operandValue);
                break;
            } else {
                inst->operands[i-2] = operandValue;
                printf("%d ", inst->operands[i-2]);
            }


            parsed_instructions++;
            i++;
        }
        printf("\n");

        if (*parsed_instructions != NULL) {
            instruction_t *next_inst = (instruction_t*)malloc(sizeof(instruction_t));            
            if (next_inst == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            inst->next = next_inst;

            build_instructions(champ, parsed_instructions, &next_inst);
        } else {
            inst->next = NULL;
        }
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