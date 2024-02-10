#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Takes 1 parameter, a register. Displays to stdout the character
corresponding to the ASCII code of the content of the register (in base
10). A 256 modulo is applied to this ASCII code
*/
int inst_aff(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
