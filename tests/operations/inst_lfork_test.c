#include <tests.h>


void inst_lfork_test(void **state) { 
	UNUSED(state);
    champion_t *champion = init_champion();
    create_champion(champion, "players/simple_2.cor");  
    
    core_t *core = init_vm();
    add_champion(core, champion);

    code_t code = 15;
    champion->counter = 100; // Set a starting counter
    int offset = 23; // Offset for the lfork
	int args[] = {offset};
    int success = inst_lfork(champion, core, code, args); // Perform the lfork
    
    assert_true(success == 0);
    assert_int_equal(core->champion_count, 2); // Verify a new champion was added
    assert_int_equal(core->champions[1].counter, (100 + offset)); // Direct address calculation
}
