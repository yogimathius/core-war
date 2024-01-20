#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void failing_test() {
	assert_false(1 != 2);
}

static void passing_test() {
	assert_false(2 != 2);
}


int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(failing_test),
		cmocka_unit_test(passing_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}