#include <tests.h>

void inst_fork_test(void **state) {
	UNUSED(state);
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_fork(champion, core, code, &inst) == 0);
}
