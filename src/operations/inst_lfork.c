#include <op.h>
#include <instructions.h>
#include <champion.h>

/* 
Same as fork without the %IDX_MOD.
*/
int inst_lfork(champion_t *champion, core_t *core, code_t code, int *inst) {
    UNUSED(core);
    UNUSED(code);
    // Calculate the new execution address without % IDX_MOD
    int new_address = champion->counter + inst[0];

    // Clone the champion to inherit the state
    champion_t *new_champ = clone_champion(champion);
    if (!new_champ) {
        return -1;
    }

    // Update the counter for the new champion to the calculated address
    new_champ->counter = new_address;

    // Add the new champion to the core
    if (add_champion_to_core(core, new_champ) != 0) {
        return -1;
    }

    return 0; // Success
};
