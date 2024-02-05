#ifndef TESTS_H_
#define TESTS_H_

// Core tests
void test_initialize_memory();
void test_read_memory();
void test_write_memory();

// Instruction tests
void inst_live_test();
void inst_ld_test();
void inst_st_test();
void inst_add_test();
void inst_sub_test();
void inst_and_test();
void inst_or_test();
void inst_xor_test();
void inst_zjmp_test();
void inst_ldi_test();
void inst_sti_test();
void inst_fork_test();
void inst_lld_test();
void inst_lldi_test();
void inst_lfork_test();
void inst_aff_test();

// Champion tests
void test_init_champion();
void test_create_champion();
void test_add_champion();
#endif
