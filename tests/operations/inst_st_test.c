#include <tests.h>

void inst_st_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
    create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
	code_t code = 3;

    // Setup: Source register (register 1) has a value to be transferred
    champ->registers[1] = 42; 
    int args[] = {1, 3, 1}; 

    inst_st(champ, core, code, args);

    // Verify the value was correctly transferred to the target register
    assert_true(champ->registers[3] == 42);
}


void inst_st_indirect_addressing_test(void **state) {
    UNUSED(state);
    core_t *core = init_vm();
    champion_t *champ = init_champion();
    create_champion(champ, "players/simple_2.cor");
    code_t code = 3;

    champ->registers[1] = 42; // Source value
    champ->counter = 100; // Assume this is the current execution point

    int offset = 34; // Offset from counter
    int args[] = {1, offset, 4}; // Source register, offset, and target type
    inst_st(champ, core, code, args); 

    // Calculate expected address using champion's counter
    int expected_address = (champ->counter + offset) % IDX_MOD;
    printf("Expected address: %d %d\n", expected_address, core->memory[expected_address]);
    assert_int_equal(core->memory[expected_address], 42); // Verify memory at expected address holds the source value
}