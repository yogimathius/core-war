#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../../include/op.h"
#include "../../include/tests.h"

void inst_live_test(void **state) { 
	UNUSED(state);
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_live(champion, core, code, &inst) == 0);
}
