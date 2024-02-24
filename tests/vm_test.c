#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/vm.h" 
#include "../include/tests.h"


void test_initialize_memory(void **state) {
    UNUSED(state);

    core_t *core_vm = init_vm();

    assert_true(core_vm != NULL);
    assert_true(core_vm->memory != NULL);
    assert_true(core_vm->champion_count == 0);
}

void test_read_memory(void **state) {
    UNUSED(state);
    core_t core_vm;

    core_vm.memory[0] = 42;
    assert_true(read_vm(&core_vm, 0) == 42);
}

void test_write_memory(void **state) {
    UNUSED(state);
    core_t core_vm;
    
    write_vm(&core_vm, 1, 24);
    assert_true(read_vm(&core_vm, 1) == 24);
}