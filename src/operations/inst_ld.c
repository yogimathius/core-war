#include <op.h>
#include <instructions.h>

/* 
Takes 2 parameters: loads the first parameter to the second parameter.
Second parameter is a register. Ex: ld 34, r3 loads the REG_SIZE bytes
starting from the Counter + 34 % IDX_MOD into r3.
*/
int inst_ld(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  int value = inst[0];
  int register_index = inst[1];

  champion->registers[register_index] = champion->counter + value % IDX_MOD;

  return 0;
};
