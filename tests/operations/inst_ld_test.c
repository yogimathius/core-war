#include <tests.h>

void inst_ld_test(void **state) {
	UNUSED(state);
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 1;
	champ->registers[2] = 2;
	core_t *core = init_vm();
	code_t code = OP_LD;
	int args[] = {32, 2};
	champ->counter = 5;

	assert_true(champ->registers[2] == 2);
	assert_true(inst_ld(champ, core, code, args) == 0);
	assert_true((int)champ->registers[2] == champ->counter + 32 % IDX_MOD);
}

void inst_ld_test_zero(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 0;
	champ->registers[2] = 0;
	core_t *core = init_vm();
	code_t code = OP_LD;
	int args[] = {0, 2};
	champ->counter = 5;

	assert_true(champ->registers[2] == 0);
	assert_true(inst_ld(champ, core, code, args) == 0);
	assert_true((int)champ->registers[2] == champ->counter + 0 % IDX_MOD);
}
