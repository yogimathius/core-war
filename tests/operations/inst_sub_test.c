#include <tests.h>

void inst_sub_test(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 1;
	champ->registers[2] = 2;
	core_t *core = init_vm();
	code_t code = 4;
	int args[] = {1, 2, 3};

	assert_true(champ->registers[3] == 0);
	assert_true(inst_sub(champ, core, code, args) == 0);
	assert_true((int)champ->registers[3] == -1);
	assert_true(champ->carry_flag == 0);
}

void inst_sub_test_zero(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 0;
	champ->registers[2] = 0;
	core_t *core = init_vm();
	code_t code = 5;
	int args[] = {1, 2, 3};

	assert_true(champ->registers[3] == 0);
	assert_true(inst_sub(champ, core, code, args) == 0);
	assert_true(champ->registers[3] == 0);
	assert_true(champ->carry_flag == 1);
}
