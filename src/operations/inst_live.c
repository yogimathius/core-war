#include <op.h>
#include <instructions.h>

/* 
Takes 1 parameter: 4 bytes that represent the player's number.
Indicates the player is alive.
*/
int inst_live(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);
  core->lives++;
  core->winner = champion->id;

  print_colored_text(champion->color);
  printf("The program P%d(%s) is alive\n\n", champion->id, champion->header.prog_name);
  printf("\033[0m");
  return 0;
};
