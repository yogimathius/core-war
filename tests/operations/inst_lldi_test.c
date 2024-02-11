#include <tests.h>

void inst_lldi_test(void **state) {
	UNUSED(state);
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_lldi(champion, core, code, &inst) == 0);
}
