#include <tests.h>


void inst_fork_test(void **state) {
	UNUSED(state);
	champion_t *champion = init_champion();
	create_champion(champion, "players/simple.cor");	
	
	core_t *core = init_vm();
	add_champion(core, champion);

	code_t code = 12;

	champion->counter = 100; // Set a starting counter

	int offset = 23; // Offset for the fork
    int args[] = {offset};
    int success = inst_fork(champion, core, code, args); // Perform the fork
	
	assert_true(success == 0);
	assert_int_equal(core->champion_count, 2); // Verify a new champion was added
    assert_int_equal(core->champions[1].counter, (100 + offset) % IDX_MOD); // Verify the new address
}