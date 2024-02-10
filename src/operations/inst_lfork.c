#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Same as fork without the %IDX_MOD.
*/
int inst_lfork(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
