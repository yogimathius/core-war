#include <tests.h>

void inst_lfork_test(void **state) { 
	UNUSED(state);
	champion_t *champion = init_champion();
	create_champion(champion, "players/simple_2.cor");	
	
	core_t *core = init_vm();
	code_t code = 1;
	int inst = 1;
	assert_true(inst_lfork(champion, core, code, &inst) == 0);
}
