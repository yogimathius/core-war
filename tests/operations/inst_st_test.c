#include <tests.h>

void inst_st_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
    create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
	code_t code = 3;

    // Setup: Source register (register 1) has a value to be transferred
    champ->registers[1] = 42; 
    int args[] = {1, 3}; 

    inst_st(champ, core, code, args);

    // Verify the value was correctly transferred to the target register
    assert_true(champ->registers[3] == 42);
}
