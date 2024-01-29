#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/tests.h"

void true_test() {
	assert_true(1 != 2);
}

void passing_test() {
	assert_false(2 != 2);
}