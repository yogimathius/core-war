#include <tests.h>

void inst_aff_test(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 1;
	champ->registers[2] = 2;
	core_t *core = init_vm();
	code_t code = OP_AFF;
	int inst[] = {1};
	assert_true(inst_aff(champ, core, code, inst) == 0);
}
