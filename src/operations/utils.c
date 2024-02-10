#include "../../include/op.h"
#include <stdio.h>

void log_instruction_args(const champion_t *champion, const core_t *core, code_t code, int *inst) {
  const op_t *operation = &op_tab[code];

  printf("=========================================\n");
  printf("called %s with:\n", op_tab[inst[0]].mnemonique);
  printf("champion: %p\n", &champion);
  printf("core: %p\n", &core);
  printf("code: %p\n", &code);
  int i = 0;
  printf("args in execute instructions: ");
  while (i < operation->nbr_args) {
    printf("%d ", inst[i]);
    i++;
  }
  printf("\n");
}