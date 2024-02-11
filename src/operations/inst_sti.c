#include <op.h>
#include <instructions.h>

/* 
Takes 3 parameters. The first one is a register. The other two can either
be indexes or registers. Ex: sti r2, %4, %5 -> copies the content for r2
into Counter + (4 + 5) % IDX_MOD.
*/
int inst_sti(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  int destination_register_index = inst[0];
  int operand1 = inst[1];
  int operand2 = inst[2];

  int result = champion->counter + (operand1 + operand2) % IDX_MOD;

  champion->registers[destination_register_index] = result;

  if (result == 0) {
      champion->carry_flag = 1;
  } else {
      champion->carry_flag = 0;
  }

  return 0;
};
