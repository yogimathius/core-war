#include <op.h>
#include <instructions.h>
#include <champion.h>

/* 
Takes 1 parameter, an index. Creates a new program, inheriting states
from the parent, and starting execution at Counter + parameter % IDX_MOD
*/
int inst_fork(champion_t *champion, core_t *core, code_t code, int *inst) {
    log_instruction_args(champion, core, code, inst);

    // Calculate the new execution address
    int new_address = (champion->counter + inst[0]) % IDX_MOD;

    // Clone the champion to inherit the state
    champion_t *new_champ = clone_champion(champion);
    if (!new_champ) {
        return -1;
    }

    // Update the counter for the new champion to the calculated address
    new_champ->counter = new_address;

    // Add the new champion to the core
    if (add_champion_to_core(core, new_champ) != 0) {
        free(new_champ);
        return -1;
    }

    return 0; // Success
}
