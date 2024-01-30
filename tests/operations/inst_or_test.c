#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../../include/op.h"
#include "../../include/tests.h"

void inst_or_test() {
	champion_t *champion = NULL;
	core_t *core = NULL;
	code_t code = 1;
	int inst = 1;
	assert_true(inst_or(champion, core, code, &inst) == 0);
}
