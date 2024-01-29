#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/op.h"
#include "../include/tests.h"

void inst_and_test() {
	assert_true(inst_and() == 0);
}
