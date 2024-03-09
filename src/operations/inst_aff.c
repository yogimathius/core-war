#include <op.h>
#include <instructions.h>

/* 
Takes 1 parameter, a register. Displays to stdout the character
corresponding to the ASCII code of the content of the register (in base
10). A 256 modulo is applied to this ASCII code
*/
int inst_aff(champion_t *champion, core_t *core, code_t code, int *inst) {
  UNUSED(core);
  UNUSED(code);
  int register_index = inst[0];
  int character = champion->registers[register_index] % 256;

  printf("%c\n", character);
  
  return 0;
};
