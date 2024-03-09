#include <op.h>
#include <instructions.h>
#include <champion.h>
#include <process.h>

/* 
Takes 1 parameter, an index. Creates a new program, inheriting states
from the parent, and starting execution at Counter + parameter % IDX_MOD
*/
int inst_fork(champion_t *champion, core_t *core, code_t code, int *inst) {
    UNUSED(code);
    int new_address = (champion->counter + inst[0]) % IDX_MOD;

    process_t *original = find_process(core, champion->id);
    process_t *new_process = clone_process(core->process, new_address);

    if (!new_process) {
        return -1;
    }

    fork_process(original, new_process);
    return 0; // Success
}
