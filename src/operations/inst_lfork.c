#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

int inst_lfork(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
