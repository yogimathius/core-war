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

void parse_instructions(const char *hex_buffer, int bytes_read, champion_t *champion) {
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
    champion->parsed_instructions = operands;
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