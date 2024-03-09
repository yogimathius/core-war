#include <op.h>

process_t *init_process(const champion_t *champion, int index) {
    process_t *process = malloc(sizeof(process_t));
    process->carry = 0;
    process->life = 0;
    process->dead = 0;
    process->counter = 0;
    process->player = champion->id;
    process->index = index;
    process->color = 0;
    process->next = NULL;

    return process;
}

void build_processes(core_t *core) {
    process_t *head = init_process(&core->champions[0], 0);
    process_t *prevptr, *new_process;

    prevptr = head;

    for (int i = 1; i < core->champion_count; i++) {
        const champion_t *champion = &core->champions[i];
        new_process = init_process(champion, (MEM_SIZE / core->champion_count) * i);
        new_process->next = NULL;
        prevptr->next = new_process;
        prevptr = new_process;
    }
    prevptr->next = head;
    core->process = head;
}