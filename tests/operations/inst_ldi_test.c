#include <tests.h>

void inst_ldi_test(void **state) {
	UNUSED(state);
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_ldi(champion, core, code, &inst) == 0);
}
