#include "../../include/op.h"
#include "../../include/instructions.h"
#include <stdio.h>

/* 
Takes 3 parameters. The first one is a register. The other two can either
be indexes or registers. Ex: sti r2, %4, %5 -> copies the content for r2
into Counter + (4 + 5) % IDX_MOD.
*/
int inst_sti(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);
  return 0;
};
