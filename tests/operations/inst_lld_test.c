#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../../include/op.h"
#include "../../include/tests.h"

void inst_lld_test(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 1;
	champ->registers[2] = 2;
	core_t *core = init_vm();
	code_t code = OP_LLD;
	int args[] = {32, 2};

	assert_true(champ->registers[2] == 2);
	assert_true(inst_lld(champ, core, code, args) == 0);
	assert_true(champ->registers[2] == 32);
	assert_true(champ->carry_flag == 0);
}

void inst_lld_test_zero(void **state) {
	UNUSED(state);
	champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");	
	// Set some registers
	champ->registers[1] = 0;
	champ->registers[2] = 0;
	core_t *core = init_vm();
	code_t code = OP_LLD;
	int args[] = {0, 2};

	printf("champ->registers[2]: %d\n", champ->registers[2]);
	assert_true(champ->registers[2] == 0);
	assert_true(inst_lld(champ, core, code, args) == 0);
	assert_true(champ->registers[2] == 0);
	assert_true(champ->carry_flag == 1);
}
