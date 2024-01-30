#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/vm.h" 
#include "../include/tests.h"

core_t virtualMachine;

void test_initialize_memory() {

    initializeVirtualMachine(&virtualMachine);
    assert_true(&virtualMachine != NULL);
    assert_true(&virtualMachine.memory != NULL);
    assert_true(&virtualMachine.registers != NULL);
}

void test_read_memory() {
    virtualMachine.registers[0] = 42;
    assert_true(readVirtualMachine(&virtualMachine, 0) == 42);
}

void test_write_memory() {
    writeVirtualMachine(&virtualMachine, 1, 24);
    assert_true(readVirtualMachine(&virtualMachine, 1) == 24);
}