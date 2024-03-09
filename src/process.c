#include <op.h>
#include <process.h>

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

process_t *find_process(core_t *core, int player) {
    process_t *head = core->process;
    process_t *current = head;
    do {
        if (current->player == player) {
          printf("Found process for player: %d\n", player);
            return current;
        }
        current = current->next;
    } while (current != head);

    return current;
}

process_t *clone_process(const process_t *original, int new_address) {
    process_t *clone = malloc(sizeof(process_t));
    clone->carry = original->carry;
    clone->life = original->life;
    clone->dead = original->dead;
    clone->counter = original->counter;
    clone->player = original->player;
    clone->index = new_address;
    clone->color = original->color;
    clone->next = original->next;
    printf("Cloned process at address: %d\n", new_address);
    return clone;
}

void fork_process(process_t *original, process_t *new_process) {
    original->next = new_process;
}