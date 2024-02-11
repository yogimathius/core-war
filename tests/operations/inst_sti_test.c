#include <tests.h>

void inst_sti_test(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 1;
	champ->registers[2] = 2;
	core_t *core = init_vm();
	code_t code = OP_STI;
	int args[] = {1, 2, 3};
	champ->counter = 0;
	assert_true(champ->registers[1] == 1);
	assert_true(inst_sti(champ, core, code, args) == 0);
	assert_true((int)champ->registers[1] == champ->counter + (2 + 3) % IDX_MOD);
	assert_true(champ->carry_flag == 0);
}

void inst_sti_test_zero(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 0;
	champ->registers[2] = 0;
	core_t *core = init_vm();
	code_t code = OP_STI;
	int args[] = {1, 2, 3};
	champ->counter = 0;
	assert_true(champ->registers[1] == 0);
	assert_true(inst_sti(champ, core, code, args) == 0);
	assert_true((int)champ->registers[1] == champ->counter + (2 + 3) % IDX_MOD);
	assert_true(champ->carry_flag == 0);
}
