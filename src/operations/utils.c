#include "../../include/op.h"

void print_colored_text(int color) { printf("\033[1;%dm", color); }

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst) {
  const op_t *operation = &op_tab[code];
  UNUSED(champion);
  UNUSED(core);
  print_colored_text(champion->color);
  printf("Champion P%d: %s called %s with: ", champion->id,champion->header.prog_name, operation->mnemonique);
  int i = 0;
  while (i < operation->nbr_args) {
    if (inst[i]) {
      printf("%d ", inst[i]);
    }
    i++;
  }
  printf("\n");

  print_colored_text(champion->color);
  printf("Cycle: %d ", core->nbr_cycles);
  printf("Cycle to die: %d ", core->cycle_to_die);
  printf("Core Lives: %d\n\n", core->lives);
  printf("\033[0m");
}

int load_value_from_address(core_t *core, int address) {
    return core->memory[address % MEM_SIZE]; 
}
