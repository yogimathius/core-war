#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/vm.h" // Include your virtual machine header file

// Define setup and teardown functions if necessary

// Test cases for initialize, read, and write functions
VirtualMachine virtualMachine;

static void test_initialize_memory(void **state) {

    initializeVirtualMachine(&virtualMachine);
    assert_true(&virtualMachine != NULL);
    assert_true(&virtualMachine.cells != NULL);
    assert_true(&virtualMachine.registers != NULL);
}

static void test_read_memory(void **state) {
    virtualMachine.registers[0] = 42;  // Load register 0 with 42
    assert_true(readVirtualMachine(&virtualMachine, 0) == 42);
}

static void test_write_memory(void **state) {
    writeVirtualMachine(&virtualMachine, 1, 24);
    assert_true(readVirtualMachine(&virtualMachine, 1) == 24);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_initialize_memory),
        cmocka_unit_test(test_read_memory),
        cmocka_unit_test(test_write_memory),
        // Add more test cases as needed
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}