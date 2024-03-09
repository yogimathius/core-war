#include <op.h>
#include <instructions.h>

/* 
Same as and, but performing an OR.
*/
int inst_or(champion_t *champion, core_t *core, code_t code, int *inst) {
  UNUSED(core);
  UNUSED(code);
  int operand1 = champion->registers[inst[0]];
  int operand2 = champion->registers[inst[1]];
  int destination_register_index = inst[2];

  int result = operand1 | operand2;

  champion->registers[destination_register_index] = result;

  if (result == 0) {
    champion->carry_flag = 1;
  } else {
    champion->carry_flag = 0;
  }

  return 0;
};
