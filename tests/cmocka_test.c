#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void true_test() {
	assert_true(1 != 2);
}

static void passing_test() {
	assert_false(2 != 2);
}


int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(true_test),
		cmocka_unit_test(passing_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}