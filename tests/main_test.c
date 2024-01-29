#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/tests.h"

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(true_test),
        cmocka_unit_test(passing_test),
        cmocka_unit_test(inst_live_test),
        cmocka_unit_test(test_initialize_memory),
        cmocka_unit_test(test_read_memory),
        cmocka_unit_test(test_write_memory),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
