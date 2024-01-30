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
        cmocka_unit_test(inst_ld_test),
        cmocka_unit_test(inst_st_test),
        cmocka_unit_test(inst_add_test),
        cmocka_unit_test(inst_sub_test),
        cmocka_unit_test(inst_and_test),
        cmocka_unit_test(inst_or_test),
        cmocka_unit_test(inst_xor_test),
        cmocka_unit_test(inst_zjmp_test),
        cmocka_unit_test(inst_ldi_test),
        cmocka_unit_test(inst_sti_test),
        cmocka_unit_test(inst_fork_test),
        cmocka_unit_test(inst_lld_test),
        cmocka_unit_test(inst_lldi_test),
        cmocka_unit_test(inst_lfork_test),
        cmocka_unit_test(inst_aff_test),
        cmocka_unit_test(test_initialize_memory),
        cmocka_unit_test(test_read_memory),
        cmocka_unit_test(test_write_memory),
        cmocka_unit_test(test_init_champion),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
