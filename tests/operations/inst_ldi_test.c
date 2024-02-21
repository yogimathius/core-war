#include <tests.h>

void inst_ldi_test(void **state) {
    UNUSED(state);
    champion_t *champ = init_champion();
	create_champion(champ, "players/simple_2.cor");
    core_t *core = init_vm();
	code_t code = 10;

    int args[] = {1, 2, 3};
    champ->registers[1] = 100; // Base address
    champ->registers[2] = 20;  // Offset

    core->memory[(100 + 20) % MEM_SIZE] = 42;

    inst_ldi(champ, core, code, args);

    assert_true(champ->registers[3] == 42);
    assert_true(champ->carry_flag == 0);
}

