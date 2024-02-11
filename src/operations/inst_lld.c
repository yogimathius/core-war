#include <op.h>
#include <instructions.h>

/* 
Same as ld without the %IDX_MOD. Modifies the carry
*/
int inst_lld(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  int value = inst[0];
  int register_index = inst[1];

  champion->registers[register_index] = value;

  if (value == 0) {
    champion->carry_flag = 1;
  } else {
    champion->carry_flag = 0;
  }

  return 0;
};
