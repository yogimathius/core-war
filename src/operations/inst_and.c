#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Same as add, and sub, but does a binary operation AND between the
first and the second, and storing the result in the third parameter.
Modifies the carry.
*/
int inst_and(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
