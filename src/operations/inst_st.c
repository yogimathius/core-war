#include "../../include/op.h"
#include "../../include/instructions.h"

#include <stdio.h>

/* 
Takes 2 parameters: Stores first parameter (a register) into the second
parameter (it can be another register or a number). Ex: st r4, 34 stores
r4 at the address of Counter + 34 % IDX_MOD; st r3, r8 copies r3 into
r8.
*/
int inst_st(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
