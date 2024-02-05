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
    assert_true(champ->id == 1);
}

void test_add_champion(void **state){
    //Initialize VM and champion
    core_t core_vm;
    init_vm(&core_vm);
    champion_t *champ = init_champion(1);

    // call add_champion with vm and champ as args
    int result = add_champion(&core_vm, champ);

    // assert vm.champion_count == 1
    assert_int_equal(core_vm.champion_count, 1);

    // assert first champion in vm list of champions is just initialized champion
    assert_true(core_vm.champions[0] == champ);

    //cleanup
    free(champ);
}

// void test_load_champion() {

// }

// void test_execute_instruction() {
//     champion_execute_instruction(&core_vm, 1, 24);
//     assert_true(read_vm(&core_vm, 1) == 24);
// }