#include "../../include/op.h"
#include <stdio.h>

int inst_fork(champion_t *champion, core_t *core, code_t code, int *inst) {
  printf("calling FORK\n");
  UNUSED(champion);
  UNUSED(core);
  UNUSED(code);
  UNUSED(inst);
  return 0;
};
