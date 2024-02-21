#include <op.h>
#include <instructions.h>

/* 
Takes 2 parameters: Stores first parameter (a register) into the second
parameter (it can be another register or a number). 
Ex: st r4, 34 stores
r4 at the address of Counter + 34 % IDX_MOD;
st r3, r8 copies r3 into
r8.
*/
int inst_st(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  int source_register = inst[0]; // The source register index
  int target = inst[1]; // The target register index or address offset

  // If the target is a register
  if (code & T_REG) {
      champion->registers[target] = champion->registers[source_register];
  } else { 
      // If the target is an address (indirect addressing)
      int address = (champion->counter + (target % IDX_MOD)) % MEM_SIZE;
      // Assuming core->memory is an array of bytes
      core->memory[address] = champion->registers[source_register];
  }

  return 0; 
};
