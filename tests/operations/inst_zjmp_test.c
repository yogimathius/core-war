#include <tests.h>

void inst_zjmp_carry_set_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
	code_t code = 9;

    // Setup: Carry flag is set
    champ->carry_flag = 1;
    champ->counter = 100; // Initial counter value
    int jump_offset = 23; // Jump offset

    // The jump should happen
    int args[] = {jump_offset};
    inst_zjmp(champ, core, code, args);

    // Verify the counter has been updated to reflect the jump
    assert_int_equal(champ->counter, (100 + jump_offset) % IDX_MOD);
}

void inst_zjmp_carry_not_set_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
	code_t code = 9;

    // Setup: Carry flag is not set
    champ->carry_flag = 0;
    champ->counter = 100; // Initial counter value
    int jump_offset = 23; // Jump offset

    // The jump should not happen
    int args[] = {jump_offset};
    inst_zjmp(champ, core, code, args);

    // Verify the counter remains unchanged, as no jump should occur
    assert_int_equal(champ->counter, 100);
}