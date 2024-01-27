#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/op.h"

static void true_test() {
	assert_true(inst_live() == 0);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(true_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}