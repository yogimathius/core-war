#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/champion.h" 
#include "../include/tests.h"

void test_champion_new() {

    champion_t *champ = champion_new();
    assert_true(champ != NULL);
}

void test_load_champion() {

}

void test_execute_instruction() {
    champion_execute_instruction(&virtualMachine, 1, 24);
    assert_true(readVirtualMachine(&virtualMachine, 1) == 24);
}