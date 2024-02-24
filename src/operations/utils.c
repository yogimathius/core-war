#include "../../include/op.h"
#include <stdio.h>

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst) {
  const op_t *operation = &op_tab[code];
  UNUSED(champion);
  UNUSED(core);
  
  printf("Champion P%d: %s called %s with: ", champion->id,champion->header.prog_name, operation->mnemonique);
  int i = 0;
  while (i < operation->nbr_args) {
    printf("%d ", inst[i]);
    i++;
  }
  printf("\n\n");
}

int load_value_from_address(core_t *core, int address) {
    return core->memory[address % MEM_SIZE]; 
}
