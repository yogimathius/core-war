#include <tests.h>

void inst_lfork_test(void **state) { 
	UNUSED(state);
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_lfork(champion, core, code, &inst) == 0);
}
