#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Same as add, but Subtracting. Modifies the carry.
*/
int inst_sub(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
