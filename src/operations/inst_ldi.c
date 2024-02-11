#include <op.h>
#include <instructions.h>

/* 
Takes 3 parameters. First two are indexes and the third one is a register
Ex: ldi 3, %4, r1 -> reads IND_SIZ bytes from address Counter + 3 % IDX_MOD, add 4 to this value (SumResult). REG_SIZE byte are read from the Counter + SumResult % IDX_MOD and copies to r1.
*/
int inst_ldi(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);

  return 0;
};
