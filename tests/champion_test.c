#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/champion.h" 
#include "../include/vm.h" 
#include "../include/tests.h"

void test_init_champion() {

    champion_t *champ = init_champion(1);
    assert_true(champ != NULL);
}

void test_add_champion() {
    core_t virtualMachine;
    init_vm(&virtualMachine);

    champion_t *champ = init_champion(1);

    add_champion(&virtualMachine, champ);
    assert_true(champ->id == 1);
    assert_true(virtualMachine.champions[0].id == champ->id);
    assert_true(virtualMachine.champion_count == 1);
    assert_true(champ->counter == 0);
    assert_true(champ->carry_flag == 0);
}

// void test_load_champion() {

// }

// void test_execute_instruction() {
//     champion_execute_instruction(&core_vm, 1, 24);
//     assert_true(read_vm(&core_vm, 1) == 24);
// }