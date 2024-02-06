#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/op.h"
#include "../include/instructions.h"

char **parse_instruction(char *instruction) {
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

void build_instructions(champion_t *champ, char **instructions, instruction_t *inst) {
    printf("Building instruction: %s\n", *instructions);
    enum op_types opcode = (enum op_types)strtoul(*instructions, NULL, 16);
    if (opcode < 1 || opcode > 16) {
        printf("No more valid instructions to parse\n");
        inst->next = NULL;
    } else {
        op_t operation = op_tab[opcode - 1];
        printf("opcode: %d, mnemonic: %s, args: %d\n", opcode, operation.mnemonique, operation.nbr_args);

        int operation_args = operation.nbr_args;
        inst->opcode = opcode;
        inst->operands = (char*)malloc((operation_args + 1) * sizeof(char));

        if (inst->operands == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        while (i < operation_args + 2) {
            if (*instructions != NULL) { 
                inst->operands[i] = **instructions;
                instructions++;
                i++;
            } else {
                break;
            }
        }

        inst->operands[i] = '\0';

        if (*instructions != NULL) {
            inst->next = (instruction_t*)malloc(sizeof(instruction_t));
            if (inst->next == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            build_instructions(champ, instructions, inst->next);
        } else {
            inst->next = NULL;
        }
    }

}