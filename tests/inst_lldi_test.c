#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/op.h"
#include "../include/tests.h"

void inst_lldi_test() {
	assert_true(inst_lldi() == 0);
}
