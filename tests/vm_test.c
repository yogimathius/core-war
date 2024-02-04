#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/vm.h" 
#include "../include/tests.h"

core_t core_vm;

void test_initialize_memory() {

    init_vm(&core_vm);
    assert_true(&core_vm != NULL);
    assert_true(&core_vm.memory != NULL);
    assert_true(&core_vm.registers != NULL);
    assert_true(core_vm.champion_count == 0);
}

void test_read_memory() {
    core_vm.registers[0] = 42;
    assert_true(read_vm(&core_vm, 0) == 42);
}

void test_write_memory() {
    write_vm(&core_vm, 1, 24);
    assert_true(read_vm(&core_vm, 1) == 24);
}