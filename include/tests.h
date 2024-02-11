#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "./op.h"
#include "./vm.h"
#include "./champion.h"

#ifndef TESTS_H_
#define TESTS_H_


// Core tests
void test_initialize_memory(void **state);
void test_read_memory(void **state);
void test_write_memory(void **state);

// Instruction tests
void inst_live_test(void **state);
void inst_ld_test(void **state);
void inst_ld_test_zero(void **state);
void inst_st_test(void **state);
void inst_add_test(void **state);
void inst_add_test_zero(void **state);
void inst_sub_test(void **state);
void inst_sub_test_zero(void **state);
void inst_and_test(void **state);
void inst_and_test_zero(void **state);
void inst_or_test(void **state);
void inst_or_test_zero(void **state);
void inst_xor_test(void **state);
void inst_xor_test_zero(void **state);
void inst_zjmp_test(void **state);
void inst_ldi_test(void **state);
void inst_sti_test(void **state);
void inst_sti_test_zero(void **state);
void inst_fork_test(void **state);
void inst_lld_test(void **state);
void inst_lld_test_zero(void **state);
void inst_lldi_test(void **state);
void inst_lfork_test(void **state);
void inst_aff_test(void **state);

// Champion tests
void test_init_champion(void **state);
void test_create_champion(void **state);
void test_add_champion(void **state);
#endif
