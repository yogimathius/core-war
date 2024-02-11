#include <op.h>
#include <instructions.h>

/* 
Takes 1 parameter, an index. Creates a new program, inheriting states
from the parent, and starting execution at Counter + parameter % IDX_MOD
*/
int inst_fork(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
