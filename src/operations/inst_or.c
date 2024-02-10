#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Same as and, but performing an OR.
*/
int inst_or(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
