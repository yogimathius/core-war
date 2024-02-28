#include <op.h>
#include <instructions.h>

void update_champion_lives(core_t *core_vm, int id) {
    for (int i = 0; i < MAX_CHAMPIONS; i++) {
        if (core_vm->champions[i].id == id) {
            core_vm->champions[i].lives++;
        }
    }
}

/* 
Takes 1 parameter: 4 bytes that represent the player's number.
Indicates the player is alive.
*/
int inst_live(champion_t *champion, core_t *core, code_t code, int *inst) {
  log_instruction_args(champion, core, code, inst);
  core->lives++;
  core->winner = champion->id;
  
  update_champion_lives(core, champion->id);
  print_colored_text(champion->color);
  printf("The program P%d(%s) is alive\n\n", champion->id, champion->header.prog_name);
  printf("\033[0m");
  return 0;
};
