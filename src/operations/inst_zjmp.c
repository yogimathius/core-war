#include <op.h>
#include <instructions.h>

/* 
Takes 1 parameter, that must be an index. Jumps to this index if carry is
1. Else, does nothing, but still consume time.
Zjmp %23 -> if carry == 1, Counter + 23 % IDX_MOD to Counter
*/

int inst_zjmp(champion_t *champion, core_t *core, code_t code, int *inst) {
  UNUSED(core);
  UNUSED(code);

  if (champion->carry_flag == 1) {
      // The parameter is an index (offset) from the current counter
      int offset = inst[0];
      champion->counter = (champion->counter + offset) % IDX_MOD;
  }

  return 0; 
};
