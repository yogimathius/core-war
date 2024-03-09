#include <op.h>
#include <instructions.h>
#include <vm.h>

/* 
Takes 2 parameters: Stores first parameter (a register) into the second
parameter (it can be another register or a number). 
Ex: st r4, 34 stores
r4 at the address of Counter + 34 % IDX_MOD;
st r3, r8 copies r3 into
r8.
*/
int inst_st(champion_t *champion, core_t *core, code_t code, int *inst) {
  UNUSED(core);
  UNUSED(code);
  int source_register = inst[0]; // The source register index
  int target = inst[1]; // The target register index or address offset

  // If the target is a register
  if (target & T_REG) {
    printf("T_REG is set: %u\n", target);
      champion->registers[target] = champion->registers[source_register];
  } else { 
      int address = (champion->counter + (target % IDX_MOD)) % MEM_SIZE;

      write_vm(core, address, champion->registers[source_register]);
  }

  return 0; 
};
