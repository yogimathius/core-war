#include <op.h>
#include <instructions.h>

/* 
Same as ldi without the %IDX_MOD. Modifies the carry
*/
int inst_lldi(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
