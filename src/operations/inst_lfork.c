#include <op.h>
#include <instructions.h>
#include <champion.h>
#include <process.h>

/* 
Same as fork without the %IDX_MOD.
*/
int inst_lfork(champion_t *champion, core_t *core, code_t code, int *inst) {
    UNUSED(core);
    UNUSED(code);
    // Calculate the new execution address without % IDX_MOD
    int new_address = champion->counter + inst[0];

    process_t *original = find_process(core, champion->id);
    process_t *new_process = clone_process(core->process, new_address);

    if (!new_process) {
        return -1;
    }


    new_process->counter = new_address;

    fork_process(original, new_process);

    return 0; // Success
};
