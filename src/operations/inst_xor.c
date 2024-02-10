#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Same as and and or, but performing an XOR.
*/
int inst_xor(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
