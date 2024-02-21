#include <tests.h>

void inst_lldi_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
    code_t code = 14;
	
    int args[] = {1, 2, 3}; 
    champ->registers[1] = 100; // Base address
    champ->registers[2] = 20;  // Offset

    core->memory[100 + 20] = 42; 

    inst_lldi(champ, core, code, args);

    // Assertions
    printf("result: %d\n", champ->registers[3]);
    assert_true(champ->registers[3] == 42); // Verifying the value was loaded into the register
    assert_true(champ->carry_flag == 0); // Verifying the carry flag is correctly updated
}