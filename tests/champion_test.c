#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>

#include "../include/champion.h" 
#include "../include/vm.h" 
#include "../include/tests.h"

void test_init_champion(void **state) {
    UNUSED(state);

    const champion_t *champ = init_champion();
    assert_true(champ != NULL);
}

void test_create_champion(void **state) {
    UNUSED(state);

    champion_t *champ = init_champion();
    create_champion(champ, "players/simple_2.cor");
    assert_true(champ->header.magic == 0xea83f3);
    assert_true(champ->header.prog_size == 23);
    assert_true(strcmp(champ->header.prog_name, "Simple") == 0);
    assert_true(strcmp(champ->header.comment, "Let's get started") == 0);
    // Add test checking instructions get loaded properly
}

void test_add_champion(void **state) {
    UNUSED(state);
    core_t *core_vm = init_vm();


    champion_t *champ = init_champion();

    add_champion(core_vm, champ);
    assert_true(champ->id == 1);
    assert_true(core_vm->champions[0].id == champ->id);
    assert_true(core_vm->champion_count == 1);
    assert_true(champ->counter == 0);
    assert_true(champ->carry_flag == 0);
}
