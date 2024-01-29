#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/op.h"
#include "../include/tests.h"

void inst_fork_test() {
	assert_true(inst_fork() == 0);
}
