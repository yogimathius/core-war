#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Takes 3 parameters: 3 registers. Add the first to the second, and store
the result to the third. Modifies the carry.
*/
int inst_add(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
