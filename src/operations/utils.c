#include <op.h>

void print_colored_text(int color) { printf("\033[1;%dm", color); }

void print_champion_registers(const champion_t *champion) {
  print_colored_text(champion->color);
  printf("Champion P%d registers: ", champion->id);
  for (int i = 0; i < REG_NUMBER; i++) {
    printf("%d ", champion->registers[i]);
  }
  printf("\n");
  printf("\033[0m");
}

void print_champion_lives(const champion_t *champion) {
  print_colored_text(champion->color);
  printf("Champion P%d lives: %d\n", champion->id, champion->lives);
  printf("\033[0m");
}

void display_champion_status(const champion_t *champion) {
  print_champion_registers(champion);
  print_champion_lives(champion);
}

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst) {
  const op_t *operation = &op_tab[code];
  print_colored_text(champion->color);
  printf("Champion P%d called %s with: ", champion->id, operation->mnemonique);
  int i = 0;
  while (i < operation->nbr_args) {
    if (inst[i]) {
      printf("%d ", inst[i]);
    }
    i++;
  }
  printf("\n");

  display_champion_status(champion);
  print_colored_text(champion->color);
  printf("Cycle: %d ", core->nbr_cycles);
  printf("Cycle to die: %d ", core->cycle_to_die);
  printf("Core Lives: %d\n\n", core->lives);
  printf("\033[0m");
}

int load_value_from_address(core_t *core, int address) {
    return core->memory[address % MEM_SIZE]; 
}



